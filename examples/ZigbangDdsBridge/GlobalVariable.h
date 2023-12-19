#include <iostream>
#include <map>
#include <string>

#define STREAM_HISTORY  8
#define BUFFER_SIZE     UXR_CONFIG_UDP_TRANSPORT_MTU* STREAM_HISTORY

class TopicInformation
{
    public:
        uint8_t topicId;
        uint16_t topic_req;
        std::string topicName;
        std::string dataType;
};

class WriterInformation
{
    public:
        uxrObjectId datawriter_id;
        uint16_t datawriter_req;
};

class zigbangUXR
{
    public: 
        std::map<int, TopicInformation> dicTopics;
        std::map<uint8_t, WriterInformation> dicWriter;
    
        // Transport Stream
        uxrUDPTransport transport;
        uxrStreamId reliable_out;
        uxrStreamId reliable_in;
        uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
        uint8_t input_reliable_stream_buffer[BUFFER_SIZE];

        // uParticipant
        uxrSession session;
        uxrObjectId participant_id;
        uint16_t participant_req;

        // Publisher
        uxrObjectId publisher_id;
        uint16_t publisher_req;

        // Subscriber
        uxrObjectId subscriber_id;
        uint16_t subscriber_req;

        

        // Reader
        uxrObjectId datareader_id;
        uint16_t datareader_req;
};