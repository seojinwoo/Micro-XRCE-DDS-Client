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

#define EXTERN
#include "global.h"

extern void zigbang_log(LogLevel level, const std::string &message);

int main(
    int args,
    char **argv)
{
    (void)args;
    (void)argv;

    if (!InitCustomTransport())
    {
        zigbang_log(LOG_FATAL, "Init Error");
        return 1;
    }
    // Transport
    uxrCustomTransport transport;
    uxr_set_custom_transport_callbacks(
        &transport,
        false,
        zigbang_custom_transport_open,
        zigbang_custom_transport_close,
        zigbang_custom_transport_write,
        zigbang_custom_transport_read);

    if (!uxr_init_custom_transport(&transport, NULL))
    {
        zigbang_log(LOG_FATAL, "Error at create transport");
        return 1;
    }

    // Session
    uxrSession session;
    uxr_init_session(&session, &transport.comm, 0xAAAABBBB);
    if (!uxr_create_session(&session))
    {
        zigbang_log(LOG_FATAL, "Error at create session.");
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
    const char *participant_xml = "<dds>"
                                  "<participant>"
                                  "<rtps>"
                                  "<name>default_xrce_participant</name>"
                                  "</rtps>"
                                  "</participant>"
                                  "</dds>";
    uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0,
                                                                 participant_xml, UXR_REPLACE);

    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    const char *topic_xml = "<dds>"
                            "<topic>"
                            "<name>TimeStamp</name>"
                            "<dataType>TimeStamp</dataType>"
                            "</topic>"
                            "</dds>";
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml,
                                                     UXR_REPLACE);

    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char *publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id,
                                                             publisher_xml, UXR_REPLACE);

    uxrObjectId datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    const char *datawriter_xml = "<dds>"
                                 "<data_writer>"
                                 "<topic>"
                                 "<kind>NO_KEY</kind>"
                                 "<name>TimeStamp</name>"
                                 "<dataType>TimeStamp</dataType>"
                                 "</topic>"
                                 "</data_writer>"
                                 "</dds>";
    uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id,
                                                               datawriter_xml, UXR_REPLACE);

    // Send create entities message and wait its status
    uint8_t status[4];
    uint16_t requests[4] = {
        participant_req, topic_req, publisher_req, datawriter_req};

    if (!uxr_run_session_until_all_status(&session, 1000, requests, status, 4))
    {
        std::string message = "Error at create entities: participant: " + std::to_string(status[0]) +
                              " topic: " + std::to_string(status[1]) +
                              " publisher: " + std::to_string(status[2]) +
                              " datawriter: " + std::to_string(status[3]);
        zigbang_log(LOG_FATAL, message);
        return 1;
    }

    // Write topics
    bool connected = true;
    uint32_t count = 0;

    struct timespec ts;
    time_t LastSec = 0;
    while (connected)
    {
        clock_gettime(CLOCK_REALTIME, &ts);

        if (LastSec != ts.tv_sec)
        {
            LastSec = ts.tv_sec;

            TimeStamp topic;
            topic.sec = (uint32_t)ts.tv_sec;
            topic.nanosec = (uint32_t)ts.tv_nsec;

            ucdrBuffer ub;
            uint32_t topic_size = TimeStamp_size_of_topic(&topic, 0);
            uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
            TimeStamp_serialize_topic(&ub, &topic);

            zigbang_log(LOG_WARN, "Send topic: sec " + std::to_string(topic.sec) + ", nano: " + std::to_string(topic.nanosec));
            connected = uxr_run_session_time(&session, 1000);
        }
    }

    // Delete resources
    uxr_delete_session(&session);
    uxr_close_custom_transport(&transport);
    DeInitCustomTransport();

    return 0;
}
