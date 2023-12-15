
#include "global.h"

void PubTask(char* ip, char* port, int index)
{
    zigbangUXR uxrPubOnly;
    uint32_t count = 0;
    threadRun.set_bit(index);

    if (uxrPubOnly.InitParticipant(ip, port, BUFFER_SIZE, BUFFER_SIZE, PARTICIPANT_ID | PARTICIPANT_ID_PUB) == false)
    {
        printf("Init Participant Error\n");
    }
    else 
    {
        // Link Topic
        {
            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            for (it = dicTopics.begin(); it != dicTopics.end(); it++)
            {
                uxrPubOnly.LinkPub(it->first, it->second.first.c_str(), it->second.second.c_str());
            }
        }

        // Send create entities message and wait its status
        while (uxrPubOnly.RegisterEntity() == false)
        {
            printf("Error at RegisterEntity Pub\n");
        }
        
        {
            bool connected = true;
            uint32_t count = 0;

            threadIdle.set_bit(index);
            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            while (connected)
            {
                for (it = dicTopics.begin(); it != dicTopics.end(); it++)
                {
                    std::string text("Hello DDS world!");

                    HelloWorld topic;
                    topic.index = ++count;
                    memset(topic.message, 0, sizeof(topic.message));
                    memcpy(topic.message, text.c_str(), text.size());

                    // uxrPubOnly.PubTopic(it->first, topic);

                    connected = uxr_run_session_time(&uxrPubOnly.session, 500);

                    if (connected == false) {
                        break;
                    }

                    if (threadRun.get_bit(EXIT_PROGRAM_ID))
                    {
                        connected = false;
                    }
                }
            }
        }
    }
    
    printf("Exit Pub Thread\n");

    threadRun.clear_bit(index);
    uxrPubOnly.Exit();
}