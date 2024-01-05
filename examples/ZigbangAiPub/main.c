// Copyright 2017 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "dds_TimeStamp.h"
#include "dds_AIFaceRecognition.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h> //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#define STREAM_HISTORY  8
#define BUFFER_SIZE     UXR_CONFIG_UDP_TRANSPORT_MTU* STREAM_HISTORY

#define TOPIC_TEST_TIMESTAMP 0

int main(
        int args,
        char** argv)
{
    // CLI
    if (3 > args || 0 == atoi(argv[2]))
    {
        printf("usage: program [-h | --help] | ip port [<max_topics>]\n");
        return 0;
    }

    char* ip = argv[1];
    char* port = argv[2];
    uint32_t max_topics = (args == 4) ? (uint32_t)atoi(argv[3]) : UINT32_MAX;

    // Transport
    uxrUDPTransport transport;
    if (!uxr_init_udp_transport(&transport, UXR_IPv4, ip, port))
    {
        printf("Error at create transport.\n");
        return 1;
    }

    // Session
    uxrSession session;
    uxr_init_session(&session, &transport.comm, 0xAAAABBBB);
    if (!uxr_create_session(&session))
    {
        printf("Error at create session.\n");
        return 1;
    }

    // Streams
    uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
    uxrStreamId reliable_out = uxr_create_output_reliable_stream(&session, output_reliable_stream_buffer, BUFFER_SIZE,
                    STREAM_HISTORY);

    uint8_t input_reliable_stream_buffer[BUFFER_SIZE];
    uxr_create_input_reliable_stream(&session, input_reliable_stream_buffer, BUFFER_SIZE, STREAM_HISTORY);

    // Create entities
    uxrObjectId participant_id = uxr_object_id(0x01, UXR_PARTICIPANT_ID);
    const char* participant_xml = "<dds>"
            "<participant>"
            "<rtps>"
            "<name>default_xrce_participant</name>"
            "</rtps>"
            "</participant>"
            "</dds>";
    uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0,
                    participant_xml, UXR_REPLACE);

    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
#if TOPIC_TEST_TIMESTAMP == 0
    const char* topic_xml = "<dds>"
            "<topic>"
            "<name>AIFaceRecognitionEvent</name>"
            "<dataType>AIFaceRecognitionEvent</dataType>"
            "</topic>"
            "</dds>";

#else
    const char* topic_xml = "<dds>"
            "<topic>"
            "<name>TimeStamp</name>"
            "<dataType>TimeStamp</dataType>"
            "</topic>"
            "</dds>";
#endif            
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml,
                    UXR_REPLACE);

    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char* publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id,
                    publisher_xml, UXR_REPLACE);

    uxrObjectId datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
#if TOPIC_TEST_TIMESTAMP == 0
    const char* datawriter_xml = "<dds>"
            "<data_writer>"
            "<topic>"
            "<kind>NO_KEY</kind>"
            "<name>AIFaceRecognitionEvent</name>"
            "<dataType>AIFaceRecognitionEvent</dataType>"
            "</topic>"
            "</data_writer>"
            "</dds>";
#else
    const char* datawriter_xml = "<dds>"
            "<data_writer>"
            "<topic>"
            "<kind>NO_KEY</kind>"
            "<name>TimeStamp</name>"
            "<dataType>TimeStamp</dataType>"
            "</topic>"
            "</data_writer>"
            "</dds>";
#endif            
    uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id,
                    datawriter_xml, UXR_REPLACE);

    // Send create entities message and wait its status
    uint8_t status[4];
    uint16_t requests[4] = {
        participant_req, topic_req, publisher_req, datawriter_req
    };
    if (!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
    {
        printf("Error at create entities: participant: %i topic: %i publisher: %i datawriter: %i\n", status[0],
                status[1], status[2], status[3]);
        return 1;
    }

    // Write topics
    bool connected = true;
    uint32_t count = 0;
    while (connected)
    {
#if TOPIC_TEST_TIMESTAMP == 0
        AIFaceRecognitionEvent topic;
        topic.confidence = 1.1 + count++;
        topic.spoofing_rate = 2.2;
        topic.user_id[0] = 'H';
        topic.user_id[1] = 'I';
        topic.user_id[2] = ',';
        topic.user_id[3] = 'J';
        topic.user_id[4] = 'W';
        topic.user_id[5] = 0;

        ucdrBuffer ub;
        uint32_t topic_size = AIFaceRecognitionEvent_size_of_topic(&topic, 0);
        printf("topic_size: %d\n", topic_size);

        uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
        AIFaceRecognitionEvent_serialize_topic(&ub, &topic);

        printf("Send topic, confidence: %f, spoofing_rate: %f, user_id: %s\n", topic.confidence, topic.spoofing_rate, topic.user_id);
#else
        TimeStamp topic;
        topic.sec = count++;
        topic.nanosec = count/2;

        ucdrBuffer ub;
        uint32_t topic_size = TimeStamp_size_of_topic(&topic, 0);
        printf("topic_size: %d\n", topic_size);

        uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
        TimeStamp_serialize_topic(&ub, &topic);

        printf("Send topic, sec: %d, nanosec: %d\n", topic.sec, topic.nanosec);
#endif

        connected = uxr_run_session_time(&session, 1000);
    }

    // Delete resources
    uxr_delete_session(&session);
    uxr_close_udp_transport(&transport);

    return 0;
}
