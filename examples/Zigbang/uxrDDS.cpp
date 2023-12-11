#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h>

#include "GlobalVariable.h"
#include "uxrDDS.h"

bool InitTrasport(zigbangUXR * uxr, char* ip, char* port)
{
    if (!uxr_init_udp_transport(&uxr->transport, UXR_IPv4, ip, port))
    {
        printf("Error at create transport.\n");
        return false;
    }
    return true;
}

bool InitSession(zigbangUXR * uxr, uint32_t key)
{
    uxr_init_session(&uxr->session, &uxr->transport.comm, key);
    if (!uxr_create_session(&uxr->session))
    {
        printf("Error at create session.\n");
        return false;
    }
    return true;
}

void MakeStream(zigbangUXR * uxr)
{
    uxr->reliable_out = uxr_create_output_reliable_stream(&uxr->session, uxr->output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    
    uxr->reliable_in = uxr_create_input_reliable_stream(&uxr->session, uxr->input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
}

void CreateEntity(zigbangUXR * uxr, uint16_t id)
{
    uxr->participant_id = uxr_object_id(id, UXR_PARTICIPANT_ID);
    const char* participant_xml = "<dds>"
            "<participant>"
            "<rtps>"
            "<name>default_xrce_participant</name>"
            "</rtps>"
            "</participant>"
            "</dds>";
    uxr->participant_req = uxr_buffer_create_participant_xml(&uxr->session, uxr->reliable_out, uxr->participant_id, 0, participant_xml, UXR_REPLACE);
}

void MakeTopic(zigbangUXR* uxr, uint8_t topicId, const char* topicName, const char* dataType)
{
    TopicInformation tempTopic;

    tempTopic.topicId = topicId;
    tempTopic.topicName = topicName;
    tempTopic.dataType = dataType;

    uxrObjectId topic_id = uxr_object_id(topicId, UXR_TOPIC_ID);

    const char* topic_xml_format = "<dds>"
                                   "<topic>"
                                   "<name>%s</name>"
                                   "<dataType>%s</dataType>"
                                   "</topic>"
                                   "</dds>";

    int formattedXmlSize = snprintf(nullptr, 0, topic_xml_format, topicName, dataType) + 1;
    char* topic_xml = new char[formattedXmlSize];
    snprintf(topic_xml, formattedXmlSize, topic_xml_format, topicName, dataType);

    tempTopic.topic_req = uxr_buffer_create_topic_xml(&uxr->session, uxr->reliable_out, topic_id, uxr->participant_id, topic_xml, UXR_REPLACE);
    uxr->dicTopics[1] = tempTopic;

    delete[] topic_xml;
}

void MakePublisher(zigbangUXR * uxr)
{
    uxr->publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char* publisher_xml = "";
    uxr->publisher_req = uxr_buffer_create_publisher_xml(&uxr->session, uxr->reliable_out, uxr->publisher_id, uxr->participant_id, publisher_xml, UXR_REPLACE);
}

void MakeSubscriber(zigbangUXR * uxr)
{
    uxr->subscriber_id = uxr_object_id(0x01, UXR_SUBSCRIBER_ID);
    const char* subscriber_xml = "";
    uint16_t subscriber_req = uxr_buffer_create_subscriber_xml(&uxr->session, uxr->reliable_out, uxr->publisher_id, uxr->participant_id, subscriber_xml, UXR_REPLACE);
}

void MakeDataWriter(zigbangUXR * uxr, uint8_t datawriterId, const char* topicName, const char* dataType)
{
    WriterInformation tempWriter;
    
    uxr->dicWriter[datawriterId] = tempWriter;

    uxr->dicWriter[datawriterId].datawriter_id = uxr_object_id(datawriterId, UXR_DATAWRITER_ID);

    const char* datawriter_xml_format = "<dds>"
                                        "<data_writer>"
                                        "<topic>"
                                        "<kind>NO_KEY</kind>"
                                        "<name>%s</name>"
                                        "<dataType>%s</dataType>"
                                        "</topic>"
                                        "</data_writer>"
                                        "</dds>";

    int formattedXmlSize = snprintf(nullptr, 0, datawriter_xml_format, topicName, dataType) + 1;
    char* datawriter_xml = new char[formattedXmlSize];
    snprintf(datawriter_xml, formattedXmlSize, datawriter_xml_format, topicName, dataType);

    uxr->dicWriter[datawriterId].datawriter_req = uxr_buffer_create_datawriter_xml(&uxr->session, uxr->reliable_out, uxr->dicWriter[datawriterId].datawriter_id, uxr->publisher_id, datawriter_xml, UXR_REPLACE);
}

void MakeDataReader(zigbangUXR * uxr, uint8_t datareaderId, const char* topicName, const char* dataType)
{
    uxr->datareader_id = uxr_object_id(0x01, UXR_DATAREADER_ID);

    const char* datareader_xml_format = "<dds>"
                                 "<data_reader>"
                                 "<topic>"
                                 "<kind>NO_KEY</kind>"
                                 "<name>%s</name>"
                                 "<dataType>%s</dataType>"
                                 "</topic>"
                                 "</data_reader>"
                                 "</dds>";
    int formattedXmlSize = snprintf(nullptr, 0, datareader_xml_format, topicName, dataType) + 1;
    char* datareader_xml = new char[formattedXmlSize];
    snprintf(datareader_xml, formattedXmlSize, datareader_xml_format, topicName, dataType);

    uxr->datareader_req = uxr_buffer_create_datareader_xml(&uxr->session, uxr->reliable_out, uxr->dicWriter[datareaderId].datawriter_id, uxr->subscriber_id, datareader_xml, UXR_REPLACE);

    delete[] datareader_xml;
}

bool RegisterEntity(zigbangUXR * uxr, uint8_t id)
{
    uint8_t status[6];
    uint16_t requests[6] = {
        uxr->participant_req, uxr->dicTopics[id].topic_req, uxr->publisher_req, uxr->dicWriter[id].datawriter_req, uxr->subscriber_req, uxr->datareader_req
    };

    if (!uxr_run_session_until_all_status(&uxr->session, 1000, requests, status, 4))
    {
        printf("Error at create entities: participant: %i topic: %i publisher: %i datawriter: %i subscriber: %i datareader: %i\n", 
                status[0], status[1], status[2], status[3], status[4], status[5]);
        return false;
    }

    return true;
}