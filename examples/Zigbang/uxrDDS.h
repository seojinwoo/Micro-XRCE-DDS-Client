#ifndef uxrDDS_H
#define uxrDDS_H

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h>

#include "global.h"
#include "HelloWorld.h"
#include <list>
#include <string>
#include <map>

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

class ReaderInformation
{
public:
    uxrObjectId datareader_id;
    uint16_t datareader_req;
};

class zigbangUXR
{
public:
    ~zigbangUXR();

    std::map<uint8_t, TopicInformation> dicTopics;
    std::map<uint8_t, WriterInformation> dicWriter;
    std::map<uint8_t, ReaderInformation> dicReader;
    std::list<std::pair<uint16_t, std::string>> listRequest;

    // Transport Stream
    uxrUDPTransport transport;
    uxrCustomTransport custom_transport;
    uxrStreamId reliable_out;
    uxrStreamId reliable_in;
    uint8_t *output_reliable_stream_buffer;
    uint8_t *input_reliable_stream_buffer;

    // uParticipant
    uxrSession session;
    uxrObjectId participant_id;
    uint16_t participant_req;

    // Publisher
    bool existPub = false;
    uxrObjectId publisher_id;
    uint16_t publisher_req;

    // Subscriber
    bool existSub = false;
    uxrObjectId subscriber_id;
    uint16_t subscriber_req;

    // debug level
    uint8_t DebugLevel = 1;

    bool InitParticipant(char *ip, char *port, uint32_t inBufferLength, uint32_t outBufferLength, uint32_t key);
    void LinkSub(uint8_t topicId, const char *topicName, const char *dataType);
    void LinkPub(uint8_t topicId, const char *topicName, const char *dataType);
    bool RegisterEntity();
    void PubTopic(uint8_t topicId, HelloWorld topic);
    void StartSubscribe(uint8_t key);
    void StartAllSubscribe();
    void Exit();

private:
    bool InitTrasport(char *ip, char *port);
    bool InitSession(uint32_t key);
    void MakeStream(uint32_t inBufferLength, uint32_t outBufferLength);
    void CreateEntity(uint16_t id);

    void MakeTopic(uint8_t topicId, const char *topicName, const char *dataType);
    void MakePublisher(uint8_t publisherId);
    void MakeDataWriter(uint8_t datawriterId, const char *topicName, const char *dataType);

    void MakeSubscriber(uint8_t publisherId);
    void MakeDataReader(uint8_t datareaderId, const char *topicName, const char *dataType);

    void AddRequest(uint16_t reqValue, std::string reqName);
};

#endif
