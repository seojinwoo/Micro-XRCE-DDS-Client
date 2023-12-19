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

#include <termios.h>
#include <unistd.h>
#include <fcntl.h> // O_RDWR, O_NOCTTY, O_NONBLOCK

#define STREAM_HISTORY 8
#define BUFFER_SIZE UXR_CONFIG_UDP_TRANSPORT_MTU *STREAM_HISTORY

#define unsigned

void SetSerial(int fd)
{
    struct termios options;
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    /* Setting CONTROL OPTIONS. */
    options.c_cflag |= unsigned(CREAD);    // Enable read.
    options.c_cflag |= unsigned(CLOCAL);   // Set local mode.
    options.c_cflag &= unsigned(~PARENB);  // Disable parity.
    options.c_cflag &= unsigned(~CSTOPB);  // Set one stop bit.
    options.c_cflag &= unsigned(~CSIZE);   // Mask the character size bits.
    options.c_cflag |= unsigned(CS8);      // Set 8 data bits.
    options.c_cflag &= unsigned(~CRTSCTS); // Disable hardware flow control.

    /* Setting LOCAL OPTIONS. */
    options.c_lflag &= unsigned(~ICANON); // Set non-canonical input.
    options.c_lflag &= unsigned(~ECHO);   // Disable echoing of input characters.
    options.c_lflag &= unsigned(~ECHOE);  // Disable echoing the erase character.
    options.c_lflag &= unsigned(~ISIG);   // Disable SIGINTR, SIGSUSP, SIGDSUSP and SIGQUIT signals.

    /* Setting INPUT OPTIONS. */
    options.c_iflag &= unsigned(~IXON);   // Disable output software flow control.
    options.c_iflag &= unsigned(~IXOFF);  // Disable input software flow control.
    options.c_iflag &= unsigned(~INPCK);  // Disable parity check.
    options.c_iflag &= unsigned(~ISTRIP); // Disable strip parity bits.
    options.c_iflag &= unsigned(~IGNBRK); // No ignore break condition.
    options.c_iflag &= unsigned(~IGNCR);  // No ignore carrier return.
    options.c_iflag &= unsigned(~INLCR);  // No map NL to CR.
    options.c_iflag &= unsigned(~ICRNL);  // No map CR to NL.

    /* Setting OUTPUT OPTIONS. */
    options.c_oflag &= unsigned(~OPOST); // Set raw output.

    /* Setting OUTPUT CHARACTERS. */
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 1;

    tcsetattr(fd, TCSANOW, &options);
}

int main(
    int args,
    char **argv)
{
    // CLI
    if (2 != args)
    {
        printf("usage: %s <dev>\n", argv[0]);
        return 1;
    }

    char *dev = argv[1];

    uint32_t max_topics = (args == 4) ? (uint32_t)atoi(argv[3]) : UINT32_MAX;

    // Transport
    uxrSerialTransport transport;
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    SetSerial(fd);

    if (!uxr_init_serial_transport(&transport, fd, 0, 1))
    {
        printf("Error during transport creation\n");
        return 1;
    }

    // Session
    uxrSession session;
    uxr_init_session(&session, &transport.comm, 0xAAAA000B);
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
    const char *participant_xml = "<dds>"
                                  "<participant>"
                                  "<rtps>"
                                  "<name>default_xrce_participant</name>"
                                  "</rtps>"
                                  "</participant>"
                                  "</dds>";
    uint16_t participant_req = uxr_buffer_create_participant_xml(&session, reliable_out, participant_id, 0,
                                                                 participant_xml, UXR_REPLACE | UXR_REUSE);

    uxrObjectId topic_id = uxr_object_id(0x01, UXR_TOPIC_ID);
    const char *topic_xml = "<dds>"
                            "<topic>"
                            "<name>HelloWorldTopic</name>"
                            "<dataType>HelloWorld</dataType>"
                            "</topic>"
                            "</dds>";
    uint16_t topic_req = uxr_buffer_create_topic_xml(&session, reliable_out, topic_id, participant_id, topic_xml,
                                                     UXR_REPLACE | UXR_REUSE);

    uxrObjectId publisher_id = uxr_object_id(0x01, UXR_PUBLISHER_ID);
    const char *publisher_xml = "";
    uint16_t publisher_req = uxr_buffer_create_publisher_xml(&session, reliable_out, publisher_id, participant_id,
                                                             publisher_xml, UXR_REPLACE | UXR_REUSE);

    uxrObjectId datawriter_id = uxr_object_id(0x01, UXR_DATAWRITER_ID);
    const char *datawriter_xml = "<dds>"
                                 "<data_writer>"
                                 "<topic>"
                                 "<kind>NO_KEY</kind>"
                                 "<name>HelloWorldTopic</name>"
                                 "<dataType>HelloWorld</dataType>"
                                 "</topic>"
                                 "</data_writer>"
                                 "</dds>";
    uint16_t datawriter_req = uxr_buffer_create_datawriter_xml(&session, reliable_out, datawriter_id, publisher_id,
                                                               datawriter_xml, UXR_REPLACE | UXR_REUSE);

    // Send create entities message and wait its status
    uint8_t status[4];
    uint16_t requests[4] = {
        participant_req, topic_req, publisher_req, datawriter_req};
    if (!uxr_run_session_until_all_status(&session, 10000, requests, status, 4))
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
        HelloWorld topic = {
            ++count, "Hello DDS world!"};

        ucdrBuffer ub;
        uint32_t topic_size = HelloWorld_size_of_topic(&topic, 0);
        uxr_prepare_output_stream(&session, reliable_out, datawriter_id, &ub, topic_size);
        HelloWorld_serialize_topic(&ub, &topic);

        printf("Send topic: %s, id: %i\n", topic.message, topic.index);
        connected = uxr_run_session_time(&session, 1000);
    }

    // Delete resources
    uxr_delete_session(&session);
    uxr_close_serial_transport(&transport);

    return 0;
}
