#ifndef GLOBAL_H
#define GLOBAL_H

#define PARTICIPANT_ID_PUB 0x0B
#define PARTICIPANT_ID_SUB 0x05

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define PARTICIPANT_ID 0xCCCC0500 // windOw5
#elif __APPLE__
#define PARTICIPANT_ID 0xCCCCAC00 // mAC
#else
#define PARTICIPANT_ID 0xCCCC1100 // lInux
#endif

#define STREAM_HISTORY  1024
#define BUFFER_SIZE     UXR_CONFIG_UDP_TRANSPORT_MTU* STREAM_HISTORY

#define MAX_REQ_ENTITY 0x10

#define PUB_STATUS_ID 0
#define SUB_STATUS_ID 1

#define EXIT_PROGRAM_ID 31


#include "uxrDDS.h"
#include "ThreadSafeBitChange.h"

#include <cstring>

#ifndef EXTERN
#define EXTERN extern
#endif

EXTERN std::map<uint8_t, std::pair<std::string, std::string>> dicTopics;
EXTERN ThreadSafeBitChange threadRun;
EXTERN ThreadSafeBitChange threadIdle;

#endif
