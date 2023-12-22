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

#include <stdio.h>  //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#include <thread>
#include <map>
#include <string>

#define EXTERN
#include "global.h"

#define TOPIC_NAME "HelloWorldTopic"
#define TOPIC2_NAME "HelloWorldTopic2"

#define TOPIC_TYPE "HelloWorld"

#define TOPIC_COUNT 100

extern void PubTask(char *ip, char *port, int index);
extern void SubTask(char *ip, char *port, int index);

int main(
    int args,
    char **argv)
{
    char *ip;
    char *port;

    // CLI
    if (3 > args || 0 == atoi(argv[2]))
    {
        ip = (char *)"192.168.1.160"; // mufia
        // ip = (char*)"192.168.100.118"; // Soma
        port = (char *)"2019";
    }
    else
    {
        ip = argv[1];
        port = argv[2];
    }

    /////////////////////////////////////////////////////////////////////////////////
    // Common
    // Make Topics
    dicTopics[1] = std::pair<std::string, std::string>{"HelloWorldTopic", TOPIC_TYPE};
    for (int i = 2; i <= TOPIC_COUNT; i++)
    {
        dicTopics[i] = std::pair<std::string, std::string>{"HelloWorldTopic" + std::to_string(i), TOPIC_TYPE};
    }

    threadRun.clear_bit(EXIT_PROGRAM_ID);

    if (true)
    {
        std::thread pubThread(PubTask, ip, port, PUB_STATUS_ID);
        while (threadRun.get_bit(PUB_STATUS_ID) != true)
            ;
        while (threadIdle.get_bit(PUB_STATUS_ID) != true)
            ;
    }

    if (true)
    {
        std::thread subThread(SubTask, ip, port, SUB_STATUS_ID);
        while (threadRun.get_bit(SUB_STATUS_ID) != true)
            ;
        while (threadIdle.get_bit(SUB_STATUS_ID) != true)
            ;
    }

    // Write topics
    bool connected = true;

    while (connected)
    {
        /*
        if ((threadRun.get_bit(PUB_STATUS_ID) == false) || (threadRun.get_bit(SUB_STATUS_ID) == false))
        {
            threadRun.set_bit(EXIT_PROGRAM_ID);
        }

        if ((threadRun.get_bit(PUB_STATUS_ID) == false) && (threadRun.get_bit(SUB_STATUS_ID) == false))
        {
            connected = false;
        }
        */
    }

    return 0;
}
