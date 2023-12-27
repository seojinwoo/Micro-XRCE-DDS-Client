// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

/*!
 * @file dds_RFID.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _dds_RFID_H_
#define _dds_RFID_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "dds_TimeStamp.h"
#include "dds_ErrorCode.h"

typedef enum RFIDReadEventType
{
    RFID_EVENT_READ,
    RFID_EVENT_FAIL
} RFIDReadEventType;

typedef uint32_t RFIDReadEventType_cdr;
typedef enum RFIDModeType
{
    RFID_MODE_NFC,
    RFID_MODE_RFID,
    RFID_MODE_ISO14443A,
    RFID_MODE_MIFARE
} RFIDModeType;

typedef uint32_t RFIDModeType_cdr;
typedef enum RFIDReadCommandType
{
    RFID_CMD_SECURITY_KEY,
    RFID_CMD_REG_USER_COUNT,
    RFID_CMD_USER_DELETE,
    RFID_CMD_USER_ALL_DELETE
} RFIDReadCommandType;

typedef uint32_t RFIDReadCommandType_cdr;
/*!
 * @brief This struct represents the structure RFIDReadEvent defined by the user in the IDL file.
 * @ingroup dds_RFID
 */
typedef struct RFIDReadEvent
{
    TimeStamp stamp;
    RFIDModeType_cdr mode;
    RFIDReadEventType_cdr event;
    char user_id[255];

} RFIDReadEvent;

struct ucdrBuffer;

bool RFIDReadEvent_serialize_topic(struct ucdrBuffer* writer, const RFIDReadEvent* topic);
bool RFIDReadEvent_deserialize_topic(struct ucdrBuffer* reader, RFIDReadEvent* topic);
uint32_t RFIDReadEvent_size_of_topic(const RFIDReadEvent* topic, uint32_t size);

/*!
 * @brief This struct represents the structure RFIDRequest defined by the user in the IDL file.
 * @ingroup dds_RFID
 */
typedef struct RFIDRequest
{
    RFIDReadCommandType_cdr command;
    char security_key[255];

} RFIDRequest;

struct ucdrBuffer;

bool RFIDRequest_serialize_topic(struct ucdrBuffer* writer, const RFIDRequest* topic);
bool RFIDRequest_deserialize_topic(struct ucdrBuffer* reader, RFIDRequest* topic);
uint32_t RFIDRequest_size_of_topic(const RFIDRequest* topic, uint32_t size);

/*!
 * @brief This struct represents the structure RFIDResponse defined by the user in the IDL file.
 * @ingroup dds_RFID
 */
typedef struct RFIDResponse
{
    RFIDReadCommandType_cdr command;
    uint16_t user_count;
    ErrorCode_cdr code;
} RFIDResponse;

struct ucdrBuffer;

bool RFIDResponse_serialize_topic(struct ucdrBuffer* writer, const RFIDResponse* topic);
bool RFIDResponse_deserialize_topic(struct ucdrBuffer* reader, RFIDResponse* topic);
uint32_t RFIDResponse_size_of_topic(const RFIDResponse* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _dds_RFID_H_