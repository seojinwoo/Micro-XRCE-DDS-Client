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
 * @file dds_AISystem.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _dds_AISystem_H_
#define _dds_AISystem_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "dds_TimeStamp.h"
#include "dds_ErrorCode.h"

typedef enum AISystemEventType
{
    AI_SYSTEM_EVENT_WAKE_UP,
    AI_SYSTEM_EVENT_SLEEP,
    AI_SYSTEM_EVENT_WIFI_CONNECT,
    AI_SYSTEM_EVENT_WIFI_DISCONNECT,
    AI_SYSTEM_EVENT_BLE_CONNECT,
    AI_SYSTEM_EVENT_BLE_DISCONNECT
} AISystemEventType;

typedef uint32_t AISystemEventType_cdr;
typedef enum AISystemCommandType
{
    AI_SYSTEM_CMD_WAKE_UP,
    AI_SYSTEM_CMD_SLEEP,
    AI_SYSTEM_CMD_WIFI_MODE,
    AI_SYSTEM_CMD_WIFI_SET_SSID,
    AI_SYSTEM_CMD_WIFI_SET_SSID_PASS,
    AI_SYSTEM_CMD_BLE_PAIRING_MODE
} AISystemCommandType;

typedef uint32_t AISystemCommandType_cdr;
/*!
 * @brief This struct represents the structure AISystemEvent defined by the user in the IDL file.
 * @ingroup dds_AISystem
 */
typedef struct AISystemEvent
{
    TimeStamp stamp;
    AISystemEventType_cdr event;
    uint8_t wifi_connection_status;
    uint8_t ble_connection_status;
} AISystemEvent;

struct ucdrBuffer;

bool AISystemEvent_serialize_topic(struct ucdrBuffer* writer, const AISystemEvent* topic);
bool AISystemEvent_deserialize_topic(struct ucdrBuffer* reader, AISystemEvent* topic);
uint32_t AISystemEvent_size_of_topic(const AISystemEvent* topic, uint32_t size);

/*!
 * @brief This struct represents the structure AISystemRequest defined by the user in the IDL file.
 * @ingroup dds_AISystem
 */
typedef struct AISystemRequest
{
    AISystemCommandType_cdr command;
    uint8_t wifi_mode;
    uint8_t ble_mode;
} AISystemRequest;

struct ucdrBuffer;

bool AISystemRequest_serialize_topic(struct ucdrBuffer* writer, const AISystemRequest* topic);
bool AISystemRequest_deserialize_topic(struct ucdrBuffer* reader, AISystemRequest* topic);
uint32_t AISystemRequest_size_of_topic(const AISystemRequest* topic, uint32_t size);

/*!
 * @brief This struct represents the structure AISystemResponse defined by the user in the IDL file.
 * @ingroup dds_AISystem
 */
typedef struct AISystemResponse
{
    AISystemCommandType_cdr command;
    ErrorCode_cdr code;
} AISystemResponse;

struct ucdrBuffer;

bool AISystemResponse_serialize_topic(struct ucdrBuffer* writer, const AISystemResponse* topic);
bool AISystemResponse_deserialize_topic(struct ucdrBuffer* reader, AISystemResponse* topic);
uint32_t AISystemResponse_size_of_topic(const AISystemResponse* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _dds_AISystem_H_