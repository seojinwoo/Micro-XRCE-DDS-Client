#define STREAM_HISTORY  8
#define BUFFER_SIZE     UXR_CONFIG_UDP_TRANSPORT_MTU* STREAM_HISTORY

typedef struct zigbangUXR
{
    uint8_t output_reliable_stream_buffer[BUFFER_SIZE];
    uint8_t input_reliable_stream_buffer[BUFFER_SIZE];

    uxrSession session;
    uxrStreamId reliable_out;
    uxrObjectId participant_id;
    uint16_t participant_req;
    uint16_t topic_req;
    uxrObjectId publisher_id;
    uint16_t publisher_req;
    uxrObjectId datawriter_id;
    uint16_t datawriter_req;
} zigbangUXR;