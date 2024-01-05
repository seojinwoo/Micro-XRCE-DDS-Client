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

#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

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

#define TOPIC_COUNT 50

extern void PubTask(char *ip, char *port, int index);
extern void SubTask(char *ip, char *port, int index);
extern void PipeTask(int index);

int main(
    int args,
    char **argv)
{
    char *ip;
    char *port;
    std::vector<std::string> TopicNames =
        {
            "AIFaceRecognitionEvent",
            "AIFaceRecognitionRequest",
            "AIFaceRecognitionResponse",

            "AIQREvent",
            "AIQRRequest",
            "AIQRResponse",

            "AISpeakerEvent",
            "AISpeakerRequest",
            "AISpeakerResponse",

            "AISystemEvent",
            "AISystemRequest",
            "AISystemResponse",

            "AIWebRTCEvent",
            "AIWebRTCRequest",
            "AIWebRTCResponse",

            "BuzzerEvent",
            "BuzzerRequest",
            "BuzzerResponse",

            "CommonResponse",

            "FingerPrintEvent",
            "FingerPrintRequest",
            "FingerPrintResponse",

            "Log",

            "MorticeEvent",
            "MorticeRequest",
            "MorticeResponse",

            "QRReadEvent",
            "QRReadRequest",
            "QRReadResponse",

            "RFIDReadEvent",
            "RFIDRequest",
            "RFIDResponse",

            "SensorEvent",
            "SensorRequest",
            "SensorResponse",

            "TimeStamp"};

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
#if true
    for (int i = 1; i <= TopicNames.size(); i++)
    {
        dicTopics[i] = std::pair<std::string, std::string>{TopicNames[i - 1], TopicNames[i - 1]};
    }
#else
    dicTopics[1] = std::pair<std::string, std::string>{"AIFaceRecognitionEvent", "AIFaceRecognitionEvent"};
#endif

    threadRun.clear_bit(EXIT_PROGRAM_ID);

    bool connected = true;

    std::thread pipeThread(PipeTask, PIPE_STATUS_ID);
    while (threadRun.get_bit(PIPE_STATUS_ID) != true)
        ;
    while (threadIdle.get_bit(PIPE_STATUS_ID) != true)
    {
        if (threadRun.get_bit(PIPE_STATUS_ID) == false)
        {
            connected = false;
            break;
        }
    }

    if (connected)
    {
        std::thread pubThread(PubTask, ip, port, PUB_STATUS_ID);
        while (threadRun.get_bit(PUB_STATUS_ID) != true)
            ;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::thread subThread(SubTask, ip, port, SUB_STATUS_ID);
        while (threadRun.get_bit(SUB_STATUS_ID) != true)
            ;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Write topics

        while (connected)
        {
            if (threadRun.get_bit(PUB_STATUS_ID) == false)
            {
                if (threadRun.get_bit(EXIT_PROGRAM_ID) == false)
                {
                    threadRun.set_bit(EXIT_PROGRAM_ID);
                }
            }
            if (threadRun.get_bit(SUB_STATUS_ID) == false)
            {
                if (threadRun.get_bit(EXIT_PROGRAM_ID) == false)
                {
                    threadRun.set_bit(EXIT_PROGRAM_ID);
                }
            }
            if (threadRun.get_bit(PIPE_STATUS_ID) == false)
            {
                if (threadRun.get_bit(EXIT_PROGRAM_ID) == false)
                {
                    threadRun.set_bit(EXIT_PROGRAM_ID);
                }
            }

            if ((threadRun.get_bit(PUB_STATUS_ID) == false) &&
                (threadRun.get_bit(SUB_STATUS_ID) == false) &&
                (threadRun.get_bit(PIPE_STATUS_ID) == false))
            {
                connected = false;
            }
        }
    }

    std::cout << "Exit ddsInterface" << std::endl;
    return 0;
}
