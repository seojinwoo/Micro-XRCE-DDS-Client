
#include "global.h"

void on_topic(
    uxrSession *session,
    uxrObjectId object_id,
    uint16_t request_id,
    uxrStreamId stream_id,
    struct ucdrBuffer *ub,
    uint16_t length,
    void *args)
{
    (void)session;
    (void)object_id;
    (void)request_id;
    (void)stream_id;
    (void)length;

    HelloWorld topic;
    HelloWorld_deserialize_topic(ub, &topic);

    // printf("Received topic: %s, id: %i\n", topic.message, topic.index);

    uint32_t *count_ptr = (uint32_t *)args;
    (*count_ptr)++;
}

void SubTask(char *ip, char *port, int index)
{
    zigbangUXR uxrSubOnly;
    uint32_t count = 0;
    threadRun.set_bit(index);

    if (uxrSubOnly.InitParticipant(ip, port, BUFFER_SIZE, BUFFER_SIZE, PARTICIPANT_ID | PARTICIPANT_ID_SUB) == false)
    {
        printf("Init Participant Error\n");
    }
    else
    {
        if (uxrSubOnly.RegisterEntity() == false)
        {
            printf("Error at RegisterEntity Sub\n");
        }

        printf("Participant ID: %x\n", PARTICIPANT_ID | PARTICIPANT_ID_SUB);

        uxr_set_topic_callback(&uxrSubOnly.session, on_topic, &count);

        // Link Topic
        {
            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            for (it = dicTopics.begin(); it != dicTopics.end(); it++)
            {
                uxrSubOnly.LinkSub(it->first, it->second.first.c_str(), it->second.second.c_str());

                if (uxrSubOnly.RegisterEntity() == false)
                {
                    printf("Error at RegisterEntity Sub\n");
                }
            }
        }

        // Send create entities message and wait its status
        /*
        while (uxrSubOnly.RegisterEntity() == false)
        {
            printf("Error at RegisterEntity Sub\n");
        }
        */

        {
            uxrSubOnly.StartAllSubscribe();
            bool connected = true;

            threadIdle.set_bit(index);
            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            while (connected)
            {
                connected = uxr_run_session_time(&uxrSubOnly.session, 1000);

                if (threadRun.get_bit(EXIT_PROGRAM_ID))
                {
                    connected = false;
                }
            }
        }
    }

    printf("Exit Sub Thread\n");

    threadRun.clear_bit(index);
    uxrSubOnly.Exit();
}