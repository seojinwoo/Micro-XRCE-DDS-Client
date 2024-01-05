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
 * @file dds_TimeStamp.c
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#include "dds_TimeStamp.h"

#include <ucdr/microcdr.h>
#include <string.h>

bool TimeStamp_serialize_topic(ucdrBuffer* writer, const TimeStamp* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, topic->sec);

        success &= ucdr_serialize_uint32_t(writer, topic->nanosec);

    return success && !writer->error;
}

bool TimeStamp_deserialize_topic(ucdrBuffer* reader, TimeStamp* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, &topic->sec);

        success &= ucdr_deserialize_uint32_t(reader, &topic->nanosec);

    return success && !reader->error;
}

uint32_t TimeStamp_size_of_topic(const TimeStamp* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}
