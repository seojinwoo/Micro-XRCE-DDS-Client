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
 * @file dds_Log.h
 * This header file contains the declaration of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#ifndef _dds_Log_H_
#define _dds_Log_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdbool.h>
#include "dds_TimeStamp.h"

typedef enum LogLevel
{
    LOG_INFO,
    LOG_WARN,
    LOG_FATAL
} LogLevel;

typedef uint32_t LogLevel_cdr;
/*!
 * @brief This struct represents the structure Log defined by the user in the IDL file.
 * @ingroup dds_Log
 */
typedef struct Log
{
    LogLevel_cdr level;
    TimeStamp stamp;
    char topic[255];

    char value[255];

} Log;

struct ucdrBuffer;

bool Log_serialize_topic(struct ucdrBuffer* writer, const Log* topic);
bool Log_deserialize_topic(struct ucdrBuffer* reader, Log* topic);
uint32_t Log_size_of_topic(const Log* topic, uint32_t size);


#ifdef __cplusplus
}
#endif

#endif // _dds_Log_H_