#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h>

#include "GlobalVariable.h"
#include "uxrDDS.h"

void MakeStream(zigbangUXR * uxr)
{
    uxr->reliable_out = uxr_create_output_reliable_stream(&uxr->session, uxr->output_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
    
    uxr_create_input_reliable_stream(&uxr->session, uxr->input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);
}

void CreateEntity(zigbangUXR * uxr)
{
    uxr->participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char* participant_xml = "<dds>"
            "<participant>"
            "<rtps>"
            "<name>default_xrce_participant</name>"
            "</rtps>"
            "</participant>"
            "</dds>";
    uxr->participant_req = uxr_buffer_create_participant_xml(&uxr->session, uxr->reliable_out, uxr->participant_id, 0, participant_xml, UXR_REPLACE);
}

void MakeTopic(zigbangUXR * uxr)
{
    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    const char* topic_xml = "<dds>"
            "<topic>"
            "<name>HelloWorldTopic</name>"
            "<dataType>HelloWorld</dataType>"
            "</topic>"
            "</dds>";
    uxr->topic_req = uxr_buffer_create_topic_xml(&uxr->session, uxr->reliable_out, topic_id, uxr->participant_id, topic_xml, UXR_REPLACE);
}

void MakePublisher(zigbangUXR * uxr)
{
    uxr->publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char* publisher_xml = "";
    uxr->publisher_req = uxr_buffer_create_publisher_xml(&uxr->session, uxr->reliable_out, uxr->publisher_id, uxr->participant_id, publisher_xml, UXR_REPLACE);
}

void MakerDataWriter(zigbangUXR * uxr)
{
    uxr->datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    const char* datawriter_xml = "<dds>"
            "<data_writer>"
            "<topic>"
            "<kind>NO_KEY</kind>"
            "<name>HelloWorldTopic</name>"
            "<dataType>HelloWorld</dataType>"
            "</topic>"
            "</data_writer>"
            "</dds>";
    uxr->datawriter_req = uxr_buffer_create_datawriter_xml(&uxr->session, uxr->reliable_out, uxr->datawriter_id, uxr->publisher_id, datawriter_xml, UXR_REPLACE);
}

bool RegisterEntity(zigbangUXR * uxr)
{
    uint8_t status[4];
    uint16_t requests[4] = {
        uxr->participant_req, uxr->topic_req, uxr->publisher_req, uxr->datawriter_req
    };
    if (!uxr_run_session_until_all_status(&uxr->session, 1000, requests, status, 4))
    {
        printf("Error at create entities: participant: %i topic: %i publisher: %i datawriter: %i\n", status[0],
                status[1], status[2], status[3]);
        return 1;
    }

    return 0;
}