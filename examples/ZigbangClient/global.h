#ifndef GLOBAL_H
#define GLOBAL_H

#define PARTICIPANT_ID_PUB 0x0B
#define PARTICIPANT_ID_SUB 0x05

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PARTICIPANT_ID 0xBBBB0500 // windOw5
#elif __APPLE__
#define PARTICIPANT_ID 0xBBBBAC00 // mAC
#else
#define PARTICIPANT_ID 0xBBBB1100 // lInux
#endif

#define STREAM_HISTORY 8
#define BUFFER_SIZE UXR_CONFIG_UDP_TRANSPORT_MTU *STREAM_HISTORY

#define MAX_REQ_ENTITY 0x10

#define PUB_STATUS_ID 0
#define SUB_STATUS_ID 1

#define EXIT_PROGRAM_ID 31

#define UXR_FRAMING_BEGIN_FLAG 0x7E
#define UXR_FRAMING_ESC_FLAG 0x7D
#define UXR_FRAMING_XOR_FLAG 0x20

#include "zigbang_custom_transport.h"

#include "ThreadSafeQueue.h"
#include "StreamPacket.h"

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

#include <uxr/client/profile/transport/custom/custom_transport.h>
#include <uxr/client/client.h>
#include <ucdr/microcdr.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <sys/poll.h>
#include <stdio.h>  //printf
#include <string.h> //strcmp
#include <stdlib.h> //atoi

#include <thread>

#ifndef EXTERN
#define EXTERN extern
#endif

EXTERN ThreadSafeQueue<std::vector<uint8_t>> RxQueue;
EXTERN ThreadSafeQueue<std::vector<uint8_t>> TxQueue;

#endif
