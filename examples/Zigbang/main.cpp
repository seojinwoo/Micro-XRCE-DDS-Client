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

#include "HelloWorld.h"

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <stdio.h> //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#include "GlobalVariable.h"
#include "uxrDDS.h"

#define PARTICIPANT_ID_PUB 0x0B
#define PARTICIPANT_ID_SUB 0x0B

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PARTICIPANT_ID 0xCCCC0100
#elif __APPLE__
    #define PARTICIPANT_ID 0xCCCC0200
#else
    #define PARTICIPANT_ID 0xCCCC0300
#endif

#define TOPIC_NAME "HelloWorldTopic"
#define TOPIC2_NAME "HelloWorldTopic2"

#define TOPIC_TYPE "HelloWorld"


void on_topic(
        uxrSession* session,
        uxrObjectId object_id,
        uint16_t request_id,
        uxrStreamId stream_id,
        struct ucdrBuffer* ub,
        uint16_t length,
        void* args)
{
    (void) session; (void) object_id; (void) request_id; (void) stream_id; (void) length;

    HelloWorld topic;
    HelloWorld_deserialize_topic(ub, &topic);

    printf("Received topic: %s, id: %i\n", topic.message, topic.index);

    uint32_t* count_ptr = (uint32_t*) args;
    (*count_ptr)++;
}

#define TOPIC_2_ENABLE 1

int main(
        int args,
        char** argv)
{
    char* ip;
    char* port;
    uint32_t count = 0;

    zigbangUXR uxr;

    // CLI
    if (3 > args || 0 == atoi(argv[2]))
    {
        // ip = (char *)"192.168.1.160";
        ip = (char *)"192.168.1.118";
        port = (char *)"2019";
    }
    else
    {
        ip = argv[1];
        port = argv[2];
    }

    // Transport
    if( InitTrasport(&uxr, ip, port) == false )
    {
        return 1;
    }

    // Session
    if( InitSession(&uxr, PARTICIPANT_ID | PARTICIPANT_ID_PUB) == false )
    {
        printf("Error at create session.\n");
        return 1;
    }
    uxr_set_topic_callback(&uxr.session, on_topic, &count);

    // Streams
    MakeStream(&uxr);

    // Create entities
    CreateEntity(&uxr, 0x01);
    
    // Make Topic
    MakeTopic(&uxr, 0x01, TOPIC_NAME, TOPIC_TYPE);
    #if (TOPIC_2_ENABLE != 0)
    MakeTopic(&uxr, 0x02, TOPIC2_NAME, TOPIC_TYPE);
    #endif
    
    // Make Pulisher
    MakePublisher(&uxr);
    
    // Make dataWriter
    MakeDataWriter(&uxr, 0x01, TOPIC_NAME, TOPIC_TYPE);
    #if (TOPIC_2_ENABLE != 0)
    MakeDataWriter(&uxr, 0x02, TOPIC2_NAME, TOPIC_TYPE);
    #endif

    // Make Subscriber
    // MakeSubscriber(&uxr);

    // Make dataReader
    // MakeDataReader(&uxr, 0x02, TOPIC_NAME, TOPIC_TYPE);
    
    // Send create entities message and wait its status
    if(RegisterEntity(&uxr, 0x01) == false)
    {
        printf("Error at RegisterEntity 0x01\n");
        return 1;
    }
    #if (TOPIC_2_ENABLE != 0)
    /*
    if(RegisterEntity(&uxr, 0x02) == false)
    {
        printf("Error at RegisterEntity 0x02\n");
        return 1;
    }
    */
    #endif
    
    // Write topics
    bool connected = true;

    while (connected)
    {
        HelloWorld topic = {
            ++count, "Hello DDS world! = HI"
        };

        ucdrBuffer ub;
        uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
        uxr_prepare_output_stream(&uxr.session, uxr.reliable_out, uxr.dicWriter[1].datawriter_id, &ub, topic_size);
        HelloWorld_serialize_topic(&ub, &topic);

        printf("Send topic1: %s, id: %i\n", topic.message, topic.index);
        connected = uxr_run_session_time(&uxr.session, 1000);

        uxr_prepare_output_stream(&uxr.session, uxr.reliable_out, uxr.dicWriter[2].datawriter_id, &ub, topic_size);
        HelloWorld_serialize_topic(&ub, &topic);

        printf("Send topic2: %s, id: %i\n", topic.message, topic.index);
        connected = uxr_run_session_time(&uxr.session, 1000);

    }

    // Delete resources
    uxr_delete_session(&uxr.session);
    uxr_close_udp_transport(&uxr.transport);

    return 0;
}

