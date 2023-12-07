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


int main(
        int args,
        char** argv)
{
    char* ip;
    char* port;
    uint32_t max_topics;

    zigbangUXR uxr;

    // CLI
    if (3 > args || 0 == atoi(argv[2]))
    {
        ip = "192.168.1.160";
        port = "2019";
        max_topics = UINT32_MAX;
    }
    else
    {
        ip = argv[1];
        port = argv[2];
        max_topics = (args == 4) ? (uint32_t)atoi(argv[3]) : UINT32_MAX;
    }

    // Transport
    uxrUDPTransport transport;
    if (!uxr_init_udp_transport(&transport, UXR_IPv4, ip, port))
    {
        printf("Error at create transport.\n");
        return 1;
    }

    // Session
    uxr_init_session(&uxr.session, &transport.comm, 0xAAAABBBB);
    if (!uxr_create_session(&uxr.session))
    {
        printf("Error at create session.\n");
        return 1;
    }

    // Streams
    MakeStream(&uxr);

    // Create entities
    CreateEntity(&uxr);
    
    // Make Topic
    MakeTopic(&uxr);
    
    // Make Pulisher
    MakePublisher(&uxr);
    
    // Make dataWriter
    MakerDataWriter(&uxr);
    
    // Send create entities message and wait its status
    if(RegisterEntity(&uxr) == 1)
    {
        return 1;
    }
    

    // Write topics
    bool connected = true;
    uint32_t count = 0;
    while (connected && count < max_topics)
    {
        HelloWorld topic = {
            ++count, "Hello DDS world! = HI"
        };

        ucdrBuffer ub;
        uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
        uxr_prepare_output_stream(&uxr.session, uxr.reliable_out, uxr.datawriter_id, &ub, topic_size);
        HelloWorld_serialize_topic(&ub, &topic);

        printf("Send topic: %s, id: %i\n", topic.message, topic.index);
        connected = uxr_run_session_time(&uxr.session, 1000);
    }

    // Delete resources
    uxr_delete_session(&uxr.session);
    uxr_close_udp_transport(&transport);

    return 0;
}

