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
 * @file dds_AIFaceRecognition.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _dds_AIFaceRecognition_H_
#define _dds_AIFaceRecognition_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "dds_TimeStamp.h"
#include "dds_ErrorCode.h"

typedef enum AIFaceRecognitionEventType
{
    FR_EVENT_MATCH,
    FR_EVENT_NOT_MATCH,
    FR_EVENT_TIMEOUT
} AIFaceRecognitionEventType;

typedef uint32_t AIFaceRecognitionEventType_cdr;
typedef enum AIFaceRecognitionCommand
{
    FR_CMD_REG_MODE,
    FR_CMD_REG_USER_COUNT,
    FR_CMD_USER_DELETE,
    FR_CMD_USER_ALL_DELETE
} AIFaceRecognitionCommand;

typedef uint32_t AIFaceRecognitionCommand_cdr;
/*!
 * @brief This struct represents the structure AIFaceRecognitionEvent defined by the user in the IDL file.
 * @ingroup dds_AIFaceRecognition
 */
typedef struct AIFaceRecognitionEvent
{
    TimeStamp stamp;
    AIFaceRecognitionEventType_cdr event;
    char user_id[255];

    float confidence;
    float spoofing_rate;
} AIFaceRecognitionEvent;

struct ucdrBuffer;

bool AIFaceRecognitionEvent_serialize_topic(struct ucdrBuffer* writer, const AIFaceRecognitionEvent* topic);
bool AIFaceRecognitionEvent_deserialize_topic(struct ucdrBuffer* reader, AIFaceRecognitionEvent* topic);
uint32_t AIFaceRecognitionEvent_size_of_topic(const AIFaceRecognitionEvent* topic, uint32_t size);

/*!
 * @brief This struct represents the structure AIFaceRecognitionRequest defined by the user in the IDL file.
 * @ingroup dds_AIFaceRecognition
 */
typedef struct AIFaceRecognitionRequest
{
    AIFaceRecognitionCommand_cdr command;
    char user_id[255];

    uint16_t timeout_sec;
} AIFaceRecognitionRequest;

struct ucdrBuffer;

bool AIFaceRecognitionRequest_serialize_topic(struct ucdrBuffer* writer, const AIFaceRecognitionRequest* topic);
bool AIFaceRecognitionRequest_deserialize_topic(struct ucdrBuffer* reader, AIFaceRecognitionRequest* topic);
uint32_t AIFaceRecognitionRequest_size_of_topic(const AIFaceRecognitionRequest* topic, uint32_t size);

/*!
 * @brief This struct represents the structure AIFaceRecognitionResponse defined by the user in the IDL file.
 * @ingroup dds_AIFaceRecognition
 */
typedef struct AIFaceRecognitionResponse
{
    AIFaceRecognitionCommand_cdr command;
    uint16_t user_count;
    ErrorCode_cdr code;
} AIFaceRecognitionResponse;

struct ucdrBuffer;

bool AIFaceRecognitionResponse_serialize_topic(struct ucdrBuffer* writer, const AIFaceRecognitionResponse* topic);
bool AIFaceRecognitionResponse_deserialize_topic(struct ucdrBuffer* reader, AIFaceRecognitionResponse* topic);
uint32_t AIFaceRecognitionResponse_size_of_topic(const AIFaceRecognitionResponse* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _dds_AIFaceRecognition_H_