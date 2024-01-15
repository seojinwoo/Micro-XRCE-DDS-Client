
#include "global.h"

// Make GetTimeStamp function which is return Json::Value with TimeStamp
Json::Value GetTimeStamp(const TimeStamp &timestamp)
{
    Json::Value root;
    root["sec"] = Json::Value(Json::objectValue);
    root["sec"]["type"] = "uint32_t";
    root["sec"]["value"] = timestamp.sec;

    root["nanosec"] = Json::Value(Json::objectValue);
    root["nanosec"]["type"] = "uint32_t";
    root["nanosec"]["value"] = timestamp.nanosec;

    return root;
}

#include "on_topic.h" // this is generated code

void SubTask(char *ip, char *port, int index)
{
    zigbangUXR uxrSubOnly;
    uint32_t count = 0;
    threadRun.set_bit(index);

    if (uxrSubOnly.InitParticipant(ip, port, BUFFER_SIZE, BUFFER_SIZE, PARTICIPANT_ID | PARTICIPANT_ID_SUB) == false)
    {
        std::cout << "Init Participant Error for Sub" << std::endl;
    }
    else
    {
        std::cout << "Participant ID: " << std::hex << (PARTICIPANT_ID | PARTICIPANT_ID_SUB) << std::dec << std::endl;

        uxr_set_topic_callback(&uxrSubOnly.session, on_topic, &count);

        // Link Topic
        {
            std::map<uint8_t, std::pair<std::string, std::string>>::iterator it;
            for (it = dicTopics.begin(); it != dicTopics.end(); it++)
            {
                uxrSubOnly.LinkSub(it->first, it->second.first.c_str(), it->second.second.c_str());
            }
        }

        // Send create entities message and wait its status
        if (uxrSubOnly.RegisterEntity() == false)
        {
            std::cout << "Error at RegisterEntity Sub" << std::endl;
        }
        else
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

    std::cout << "Exit Sub Thread" << std::endl;

    threadRun.clear_bit(index);
    uxrSubOnly.Exit();
}