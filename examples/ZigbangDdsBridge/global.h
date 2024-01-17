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

#define STREAM_HISTORY 1024
#define BUFFER_SIZE UXR_CONFIG_UDP_TRANSPORT_MTU *STREAM_HISTORY

#define MAX_REQ_ENTITY 0x10

#define PUB_STATUS_ID 0
#define SUB_STATUS_ID 1

#define EXIT_PROGRAM_ID 31

#define UXR_FRAMING_BEGIN_FLAG 0x7E
#define UXR_FRAMING_ESC_FLAG 0x7D
#define UXR_FRAMING_XOR_FLAG 0x20

#include "ThreadSafeBitChange.h"
#include "ThreadSafeQueue.h"
#include "StreamPacket.h"

#include "dds_Log.h"

#include <boost/asio.hpp>
#include <boost/asio/serial_port.hpp>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <vector>
#include <chrono>
#include <thread>
#include <cstring>
#include <iomanip>
#include <iostream>

#ifndef EXTERN
#define EXTERN extern
#endif

extern void zigbang_log(LogLevel level, const std::string &message);

EXTERN std::map<uint8_t, std::pair<std::string, std::string>> dicTopics;
EXTERN ThreadSafeBitChange threadRun;
EXTERN ThreadSafeBitChange threadIdle;
EXTERN ThreadSafeQueue<std::vector<uint8_t>> TxUdpQueue;
EXTERN ThreadSafeQueue<std::vector<uint8_t>> TxSerialQueue[2];
EXTERN ThreadSafeQueue<uint8_t> RxQueue;
#endif
