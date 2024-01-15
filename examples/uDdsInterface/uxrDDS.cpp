#include "global.h"
#include "uxrDDS.h"

bool zigbangUXR::InitTrasport(char *ip, char *port)
{
    std::cout << "ip: " << ip << ", port: " << port << std::endl;
    if (!uxr_init_udp_transport(&transport, UXR_IPv4, ip, port))
    {
        std::cout << "Error at create transport." << std::endl;
        return false;
    }
    return true;
}

bool zigbangUXR::InitSession(uint32_t key)
{
    uxr_init_session(&session, &transport.comm, key);
    if (!uxr_create_session(&session))
    {
        std::cout << "Error at create session." << std::endl;
        return false;
    }
    return true;
}

void zigbangUXR::MakeStream(uint32_t inBufferLength, uint32_t outBufferLength)
{
    output_reliable_stream_buffer = new uint8_t[outBufferLength];
    input_reliable_stream_buffer = new uint8_t[inBufferLength];

    reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, outBufferLength, outBufferLength / UXR_CONFIG_UDP_TRANSPORT_MTU);
    reliable_in = uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, inBufferLength, inBufferLength / UXR_CONFIG_UDP_TRANSPORT_MTU);
}

void zigbangUXR::CreateEntity(uint16_t id)
{
    participant_id = uxr_object_id(id, UXR_PARTICIPANT_ID);
    const char *participant_xml = "<dds>"
                                  "<participant>"
                                  "<rtps>"
                                  "<name>default_xrce_participant</name>"
                                  "</rtps>"
                                  "</participant>"
                                  "</dds>";
    participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0, participant_xml, UXR_REPLACE);

    AddRequest(participant_req, "participant_req(id:" + std::to_string(id) + ")");
}

void zigbangUXR::MakeTopic(uint8_t topicId, const char *topicName, const char *dataType)
{
    TopicInformation tempTopic;

    tempTopic.topicId = topicId;
    tempTopic.topicName = topicName;
    tempTopic.dataType = dataType;

    uxrObjectId topic_id = uxr_object_id(topicId, UXR_TOPIC_ID);

    const char *topic_xml_format = "<dds>"
                                   "<topic>"
                                   "<name>%s</name>"
                                   "<dataType>%s</dataType>"
                                   "</topic>"
                                   "</dds>";

    int formattedXmlSize = snprintf(nullptr, 0, topic_xml_format, topicName, dataType) + 1;
    char *topic_xml = new char[formattedXmlSize];
    snprintf(topic_xml, formattedXmlSize, topic_xml_format, topicName, dataType);

    tempTopic.topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml, UXR_REPLACE);
    dicTopics[topicId] = tempTopic;

    AddRequest(tempTopic.topic_req, "topic_req(id:" + std::to_string(topicId) + ")");

    delete[] topic_xml;
}

void zigbangUXR::MakePublisher(uint8_t publisherId)
{
    publisher_id = uxr_object_id(publisherId, UXR_PUBLISHER_ID);
    const char *publisher_xml = "";
    publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id, publisher_xml, UXR_REPLACE);

    AddRequest(publisher_req, "publisher_req(id:" + std::to_string(publisherId) + ")");
}

void zigbangUXR::MakeSubscriber(uint8_t subscriberId)
{
    subscriber_id = uxr_object_id(subscriberId, UXR_SUBSCRIBER_ID);
    const char *subscriber_xml = "";
    subscriber_req = uxr_buffer_create_subscriber_xml(&session, reliable_out, subscriber_id, participant_id, subscriber_xml, UXR_REPLACE);

    AddRequest(subscriber_req, "subscriber_req(id:" + std::to_string(subscriberId) + ")");
}

void zigbangUXR::MakeDataWriter(uint8_t datawriterId, const char *topicName, const char *dataType)
{
    WriterInformation tempWriter;

    tempWriter.datawriter_id = uxr_object_id(datawriterId, UXR_DATAWRITER_ID);

    const char *datawriter_xml_format = "<dds>"
                                        "<data_writer>"
                                        "<topic>"
                                        "<kind>NO_KEY</kind>"
                                        "<name>%s</name>"
                                        "<dataType>%s</dataType>"
                                        "</topic>"
                                        "</data_writer>"
                                        "</dds>";

    int formattedXmlSize = snprintf(nullptr, 0, datawriter_xml_format, topicName, dataType) + 1;
    char *datawriter_xml = new char[formattedXmlSize];
    snprintf(datawriter_xml, formattedXmlSize, datawriter_xml_format, topicName, dataType);

    tempWriter.datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, tempWriter.datawriter_id, publisher_id, datawriter_xml, UXR_REPLACE);

    dicWriter[topicName] = tempWriter;

    AddRequest(tempWriter.datawriter_req, "datawriter_req(id:" + std::to_string(datawriterId) + ")");

    delete[] datawriter_xml;
}

void zigbangUXR::MakeDataReader(uint8_t datareaderId, const char *topicName, const char *dataType)
{
    ReaderInformation tempReader;

    tempReader.datareader_id = uxr_object_id(datareaderId, UXR_DATAREADER_ID);

    const char *datareader_xml_format = "<dds>"
                                        "<data_reader>"
                                        "<topic>"
                                        "<kind>NO_KEY</kind>"
                                        "<name>%s</name>"
                                        "<dataType>%s</dataType>"
                                        "</topic>"
                                        "</data_reader>"
                                        "</dds>";
    int formattedXmlSize = snprintf(nullptr, 0, datareader_xml_format, topicName, dataType) + 1;
    char *datareader_xml = new char[formattedXmlSize];
    snprintf(datareader_xml, formattedXmlSize, datareader_xml_format, topicName, dataType);

    tempReader.datareader_req = uxr_buffer_create_datareader_xml(&session, reliable_out, tempReader.datareader_id, subscriber_id, datareader_xml, UXR_REPLACE);

    dicReader[datareaderId] = tempReader;

    AddRequest(tempReader.datareader_req, "datareader_req(id:" + std::to_string(datareaderId) + ")");

    delete[] datareader_xml;
}

bool zigbangUXR::RegisterEntity()
{
    bool ReturnValue = true;
#if true
    int size = listRequest.size();
    if (size > MAX_REQ_ENTITY)
    {
        size = MAX_REQ_ENTITY;
    }
    uint16_t *requests = new uint16_t[size];
    uint8_t *status = new uint8_t[size];

    int i = 0;
    for (auto it = listRequest.begin(); it != listRequest.end(); ++it)
    {
        requests[i] = it->first;
        ++i;
        if (i >= size)
        {
            break;
        }
    }

    if (!uxr_run_session_until_all_status(&session, 1000, requests, status, size))
    {
        i = 0;
        std::string error_message = "Error at create entities: \r\n";
        for (auto it = listRequest.begin(); it != listRequest.end(); ++it)
        {
            error_message += it->second + ": " + std::to_string(status[it->first]) + "\r\n";
            ++i;
            if (i >= size)
            {
                break;
            }
        }
        printf("%s", error_message.c_str());
        ReturnValue = false;
    }

    delete[] requests;
    delete[] status;
#else
    int i = 0;
    uint16_t requests;
    uint8_t status;

    for (auto it = listRequest.begin(); it != listRequest.end(); ++it)
    {
        requests = it->first;
        status = 0;
        if (!uxr_run_session_until_confirm_delivery(&session, 1000))
        {
            std::string error_message = "Error at create entities: ";
            error_message += it->second + ": " + std::to_string(status) + "\r\n";
            printf("%s", error_message.c_str());
            // ReturnValue = false;
        }
    }
#endif
    return ReturnValue;
}

zigbangUXR::~zigbangUXR()
{
    delete[] output_reliable_stream_buffer;
    delete[] input_reliable_stream_buffer;
}

bool zigbangUXR::InitParticipant(char *ip, char *port, uint32_t inBufferLength, uint32_t outBufferLength, uint32_t key)
{
    // Transport
    if (InitTrasport(ip, port) == false)
    {
        return false;
    }

    // Session
    if (InitSession(key) == false)
    {
        std::cout << "Error at create session" << std::endl;
        return false;
    }

    // Streams
    MakeStream(inBufferLength, outBufferLength);

    // Create entities
    CreateEntity(0x01);

    return true;
}

void zigbangUXR::LinkSub(uint8_t topicId, const char *topicName, const char *dataType)
{
    if (DebugLevel != 0)
    {
        // printf("Link Sub %d, %s, %s\n", topicId, topicName, dataType);
        std::cout << "Link Sub " << std::to_string(topicId) << ", " << topicName << ", " << dataType << std::endl;
    }

    // Make Pulisher
    if (existSub == false)
    {
        existSub = true;
        MakeSubscriber(topicId);
    }

    // Make Topic
    MakeTopic(topicId, topicName, dataType);

    // Make dataReader
    MakeDataReader(topicId, topicName, dataType);
}

void zigbangUXR::LinkPub(uint8_t topicId, const char *topicName, const char *dataType)
{
    if (DebugLevel != 0)
    {
        // printf("Link Pub %d, %s, %s\n", topicId, topicName, dataType);

        std::cout << "Link Pub " << std::to_string(topicId) << ", " << topicName << ", " << dataType << std::endl;
    }

    // Make Pulisher
    if (existPub == false)
    {
        existPub = true;
        MakePublisher(topicId);
    }

    // Make Topic
    MakeTopic(topicId, topicName, dataType);

    // Make dataWriter
    MakeDataWriter(topicId, topicName, dataType);
}

void zigbangUXR::StartAllSubscribe()
{
    std::map<uint8_t, TopicInformation>::iterator it;
    for (it = dicTopics.begin(); it != dicTopics.end(); it++)
    {
        StartSubscribe(it->first);
    }
}

void zigbangUXR::StartSubscribe(uint8_t key)
{
    uxrDeliveryControl delivery_control = {
        0};
    delivery_control.max_samples = UXR_MAX_SAMPLES_UNLIMITED;
    uxr_buffer_request_data(&session, reliable_out, dicReader[key].datareader_id, reliable_in, &delivery_control);
}

void zigbangUXR::AddRequest(uint16_t reqValue, std::string reqName)
{
    listRequest.push_back(std::make_pair(reqValue, reqName));
}

void zigbangUXR::Exit()
{
    uxr_delete_session(&session);
    uxr_close_udp_transport(&transport);
}

void zigbangUXR::PubTopic(AIFaceRecognitionEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIFaceRecognitionEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIFaceRecognitionEvent"].datawriter_id, &ub, topic_size);
    AIFaceRecognitionEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIFaceRecognitionRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIFaceRecognitionRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIFaceRecognitionRequest"].datawriter_id, &ub, topic_size);
    AIFaceRecognitionRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIFaceRecognitionResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIFaceRecognitionResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIFaceRecognitionResponse"].datawriter_id, &ub, topic_size);
    AIFaceRecognitionResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIQREvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIQREvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIQREvent"].datawriter_id, &ub, topic_size);
    AIQREvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIQRRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIQRRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIQRRequest"].datawriter_id, &ub, topic_size);
    AIQRRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIQRResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIQRResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIQRResponse"].datawriter_id, &ub, topic_size);
    AIQRResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AISpeakerEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AISpeakerEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AISpeakerEvent"].datawriter_id, &ub, topic_size);
    AISpeakerEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AISpeakerRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AISpeakerRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AISpeakerRequest"].datawriter_id, &ub, topic_size);
    AISpeakerRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AISpeakerResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AISpeakerResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AISpeakerResponse"].datawriter_id, &ub, topic_size);
    AISpeakerResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AISystemEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AISystemEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AISystemEvent"].datawriter_id, &ub, topic_size);
    AISystemEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AISystemRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AISystemRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AISystemRequest"].datawriter_id, &ub, topic_size);
    AISystemRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AISystemResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AISystemResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AISystemResponse"].datawriter_id, &ub, topic_size);
    AISystemResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIWebRTCEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIWebRTCEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIWebRTCEvent"].datawriter_id, &ub, topic_size);
    AIWebRTCEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIWebRTCRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIWebRTCRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIWebRTCRequest"].datawriter_id, &ub, topic_size);
    AIWebRTCRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(AIWebRTCResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = AIWebRTCResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["AIWebRTCResponse"].datawriter_id, &ub, topic_size);
    AIWebRTCResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(BuzzerEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = BuzzerEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["BuzzerEvent"].datawriter_id, &ub, topic_size);
    BuzzerEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(BuzzerRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = BuzzerRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["BuzzerRequest"].datawriter_id, &ub, topic_size);
    BuzzerRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(BuzzerResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = BuzzerResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["BuzzerResponse"].datawriter_id, &ub, topic_size);
    BuzzerResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(CommonResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = CommonResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["CommonResponse"].datawriter_id, &ub, topic_size);
    CommonResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(FingerPrintEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = FingerPrintEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["FingerPrintEvent"].datawriter_id, &ub, topic_size);
    FingerPrintEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(FingerPrintRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = FingerPrintRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["FingerPrintRequest"].datawriter_id, &ub, topic_size);
    FingerPrintRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(FingerPrintResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = FingerPrintResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["FingerPrintResponse"].datawriter_id, &ub, topic_size);
    FingerPrintResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(Log topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = Log_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["Log"].datawriter_id, &ub, topic_size);
    Log_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(MorticeEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = MorticeEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["MorticeEvent"].datawriter_id, &ub, topic_size);
    MorticeEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(MorticeRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = MorticeRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["MorticeRequest"].datawriter_id, &ub, topic_size);
    MorticeRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(MorticeResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = MorticeResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["MorticeResponse"].datawriter_id, &ub, topic_size);
    MorticeResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(QRReadEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = QRReadEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["QRReadEvent"].datawriter_id, &ub, topic_size);
    QRReadEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(QRReadRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = QRReadRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["QRReadRequest"].datawriter_id, &ub, topic_size);
    QRReadRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(QRReadResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = QRReadResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["QRReadResponse"].datawriter_id, &ub, topic_size);
    QRReadResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(RFIDReadEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = RFIDReadEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["RFIDReadEvent"].datawriter_id, &ub, topic_size);
    RFIDReadEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(RFIDRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = RFIDRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["RFIDRequest"].datawriter_id, &ub, topic_size);
    RFIDRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(RFIDResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = RFIDResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["RFIDResponse"].datawriter_id, &ub, topic_size);
    RFIDResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(SensorEvent topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = SensorEvent_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["SensorEvent"].datawriter_id, &ub, topic_size);
    SensorEvent_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(SensorRequest topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = SensorRequest_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["SensorRequest"].datawriter_id, &ub, topic_size);
    SensorRequest_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(SensorResponse topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = SensorResponse_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["SensorResponse"].datawriter_id, &ub, topic_size);
    SensorResponse_serialize_topic(&ub, &topic);
}

void zigbangUXR::PubTopic(TimeStamp topic)
{
    ucdrBuffer ub;
    uint32_t topic_size = TimeStamp_size_of_topic(&topic, 0);
    uxr_prepare_output_stream(&session, reliable_out, dicWriter["TimeStamp"].datawriter_id, &ub, topic_size);
    TimeStamp_serialize_topic(&ub, &topic);
}