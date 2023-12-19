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

#include <stdio.h>  //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#include <thread>
#include <map>
#include <string>

#define EXTERN
#include "global.h"

extern void SerialTask(char *dev, int index);
extern void UdpTask(char *ip, char *port, int index);

void testCode()
{
    std::vector<uint8_t> toSend =
        {
            //   00    01    02    03    04    05    06    07    08    09
            0x7e, 0x01, 0x00, 0x18, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, // 00
            0x01, 0x10, 0x00, 0x58, 0x52, 0x43, 0x45, 0x01, 0x00, 0x01, // 10
            0x0f, 0xaa, 0xaa, 0x00, 0x0b, 0x81, 0x00, 0xfc, 0x01, 0x67, // 20
            0x90                                                        // 30
        };
    // TxUdpQueue.push(toSend);
}

int main(
    int args,
    char **argv)
{
    // udp
    char *ip;
    char *port;
    // serial
    char *dev;

    ip = (char *)"192.168.1.160"; // mufia
    // ip = (char*)"192.168.100.118"; // Soma

    port = (char *)"2019";

    dev = (char *)"/dev/ttyUSB0";

    std::thread serialThread(SerialTask, dev, PUB_STATUS_ID);
    std::thread udpThread(UdpTask, ip, port, PUB_STATUS_ID);

    bool connected = true;

    testCode();

    while (connected)
    {
    }

    return 0;
}
