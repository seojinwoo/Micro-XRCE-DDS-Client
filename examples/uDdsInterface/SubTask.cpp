
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

AIFaceRecognitionEvent topic;

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

    // HelloWorld topic;
    // HelloWorld_deserialize_topic(ub, &topic);

    // std::cout << "Received topic - object_id:, " << object_id.id << ", request_id: " << request_id << ", stream_id: " << stream_id.index << std::endl;

    if (object_id.id == 1)
    {
        AIFaceRecognitionEvent_deserialize_topic(ub, &topic);

        std::cout << "Received : AIFaceRecognitionEvent - confidence: " << topic.confidence << ", spoofing_rate: " << topic.spoofing_rate << ", user_id: " << topic.user_id << std::endl;

        /*
        // make json from topic
        Json::Value root;
        // Add name and value to the object
        root["TopicName"] = "AIFaceRecognitionEvent";

        root["AIFaceRecognitionEvent"]["stamp"] = Json::Value(Json::objectValue);
        root["AIFaceRecognitionEvent"]["stamp"]["type"] = "uint32_t";
        root["AIFaceRecognitionEvent"]["stamp"]["value"] = GetTimeStamp(topic.stamp);

        root["AIFaceRecognitionEvent"]["event"] = Json::Value(Json::objectValue);
        root["AIFaceRecognitionEvent"]["event"]["type"] = "uint32_t";
        root["AIFaceRecognitionEvent"]["event"]["value"] = (uint32_t)topic.event;

        root["AIFaceRecognitionEvent"]["user_id"] = Json::Value(Json::objectValue);
        root["AIFaceRecognitionEvent"]["user_id"]["type"] = "string";
        root["AIFaceRecognitionEvent"]["user_id"]["value"] = std::string(topic.user_id);

        root["AIFaceRecognitionEvent"]["confidence"] = Json::Value(Json::objectValue);
        root["AIFaceRecognitionEvent"]["confidence"]["type"] = "float";
        root["AIFaceRecognitionEvent"]["confidence"]["value"] = topic.confidence;

        root["AIFaceRecognitionEvent"]["spoofing_rate"] = Json::Value(Json::objectValue);
        root["AIFaceRecognitionEvent"]["spoofing_rate"]["type"] = "float";
        root["AIFaceRecognitionEvent"]["spoofing_rate"]["value"] = topic.spoofing_rate;

        // send json to server
        Json::FastWriter fastWriter;
        std::string ToPush = fastWriter.write(root);
        fromSub.push(ToPush);
        */
    }
    else if (object_id.id == 36)
    {
        TimeStamp topic;
        TimeStamp_deserialize_topic(ub, &topic);

        std::cout << "Received : TimeStamp - sec: " << topic.sec << ", nanosec: " << topic.nanosec << std::endl;

        // make json from topic
        Json::Value root;
        // Add name and value to the object
        root["TopicName"] = "TimeStamp";

        root["TimeStamp"]["sec"] = Json::Value(Json::objectValue);
        root["TimeStamp"]["sec"]["type"] = "uint32_t";
        root["TimeStamp"]["sec"]["value"] = topic.sec;

        root["TimeStamp"]["nanosec"] = Json::Value(Json::objectValue);
        root["TimeStamp"]["nanosec"]["type"] = "uint32_t";
        root["TimeStamp"]["nanosec"]["value"] = topic.nanosec;

        // send json to server
        Json::FastWriter fastWriter;
        std::string ToPush = fastWriter.write(root);
        fromSub.push(ToPush);
    }
    else
    {
        std::string ToPush("Received topic: ");
        ToPush += std::to_string(object_id.id);
        fromSub.push(ToPush);
        std::cout << ToPush << std::endl;
    }

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