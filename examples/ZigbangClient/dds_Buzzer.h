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
 * @file dds_Buzzer.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _dds_Buzzer_H_
#define _dds_Buzzer_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "dds_TimeStamp.h"
#include "dds_ErrorCode.h"

typedef enum BuzzerEventType
{
    BUZZER_EVENT_START,
    BUZZER_EVENT_STOP
} BuzzerEventType;

typedef uint32_t BuzzerEventType_cdr;
typedef enum BuzzerCommandType
{
    BUZZER_CMD_START,
    BUZZER_CMD_STOP
} BuzzerCommandType;

typedef uint32_t BuzzerCommandType_cdr;
/*!
 * @brief This struct represents the structure BuzzerEvent defined by the user in the IDL file.
 * @ingroup dds_Buzzer
 */
typedef struct BuzzerEvent
{
    TimeStamp stamp;
    BuzzerEventType_cdr event;
} BuzzerEvent;

struct ucdrBuffer;

bool BuzzerEvent_serialize_topic(struct ucdrBuffer* writer, const BuzzerEvent* topic);
bool BuzzerEvent_deserialize_topic(struct ucdrBuffer* reader, BuzzerEvent* topic);
uint32_t BuzzerEvent_size_of_topic(const BuzzerEvent* topic, uint32_t size);

/*!
 * @brief This struct represents the structure BuzzerRequest defined by the user in the IDL file.
 * @ingroup dds_Buzzer
 */
typedef struct BuzzerRequest
{
    BuzzerCommandType_cdr command;
    uint16_t buzzer_type;
} BuzzerRequest;

struct ucdrBuffer;

bool BuzzerRequest_serialize_topic(struct ucdrBuffer* writer, const BuzzerRequest* topic);
bool BuzzerRequest_deserialize_topic(struct ucdrBuffer* reader, BuzzerRequest* topic);
uint32_t BuzzerRequest_size_of_topic(const BuzzerRequest* topic, uint32_t size);

/*!
 * @brief This struct represents the structure BuzzerResponse defined by the user in the IDL file.
 * @ingroup dds_Buzzer
 */
typedef struct BuzzerResponse
{
    BuzzerCommandType_cdr command;
    ErrorCode_cdr code;
} BuzzerResponse;

struct ucdrBuffer;

bool BuzzerResponse_serialize_topic(struct ucdrBuffer* writer, const BuzzerResponse* topic);
bool BuzzerResponse_deserialize_topic(struct ucdrBuffer* reader, BuzzerResponse* topic);
uint32_t BuzzerResponse_size_of_topic(const BuzzerResponse* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _dds_Buzzer_H_