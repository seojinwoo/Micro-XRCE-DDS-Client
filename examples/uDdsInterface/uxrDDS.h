#ifndef uxrDDS_H
#define uxrDDS_H

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

// #include <stdio.h>

#include "global.h"

#include "dds_AIFaceRecognition.h"
#include "dds_AIQR.h"
#include "dds_AISpeaker.h"
#include "dds_AISystem.h"
#include "dds_AIWebRTC.h"
#include "dds_Buzzer.h"
#include "dds_Common.h"
#include "dds_ErrorCode.h"
#include "dds_FingerPrint.h"
#include "dds_Log.h"
#include "dds_Mortice.h"
#include "dds_QR.h"
#include "dds_RFID.h"
#include "dds_Sensor.h"
#include "dds_TimeStamp.h"

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
    std::map<std::string, WriterInformation> dicWriter;
    std::map<uint8_t, ReaderInformation> dicReader;
    std::list<std::pair<uint16_t, std::string>> listRequest;

    // Transport Stream
    uxrUDPTransport transport;
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
    void StartSubscribe(uint8_t key);
    void StartAllSubscribe();
    void Exit();

    // For paste Generated Code here - Start
    void PubTopic(AIFaceRecognitionEvent topic);
    void PubTopic(AIFaceRecognitionRequest topic);
    void PubTopic(AIFaceRecognitionResponse topic);
    void PubTopic(AIQREvent topic);
    void PubTopic(AIQRRequest topic);
    void PubTopic(AIQRResponse topic);
    void PubTopic(AISpeakerEvent topic);
    void PubTopic(AISpeakerRequest topic);
    void PubTopic(AISpeakerResponse topic);
    void PubTopic(AISystemEvent topic);
    void PubTopic(AISystemRequest topic);
    void PubTopic(AISystemResponse topic);
    void PubTopic(AIWebRTCEvent topic);
    void PubTopic(AIWebRTCRequest topic);
    void PubTopic(AIWebRTCResponse topic);
    void PubTopic(BuzzerEvent topic);
    void PubTopic(BuzzerRequest topic);
    void PubTopic(BuzzerResponse topic);
    void PubTopic(CommonResponse topic);
    void PubTopic(FingerPrintEvent topic);
    void PubTopic(FingerPrintRequest topic);
    void PubTopic(FingerPrintResponse topic);
    void PubTopic(Log topic);
    void PubTopic(MorticeEvent topic);
    void PubTopic(MorticeRequest topic);
    void PubTopic(MorticeResponse topic);
    void PubTopic(QRReadEvent topic);
    void PubTopic(QRReadRequest topic);
    void PubTopic(QRReadResponse topic);
    void PubTopic(RFIDReadEvent topic);
    void PubTopic(RFIDRequest topic);
    void PubTopic(RFIDResponse topic);
    void PubTopic(SensorEvent topic);
    void PubTopic(SensorRequest topic);
    void PubTopic(SensorResponse topic);
    void PubTopic(TimeStamp topic);
    // For paste Generated Code here - End

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
