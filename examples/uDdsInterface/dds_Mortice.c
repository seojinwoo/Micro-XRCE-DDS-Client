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
 * @file dds_Mortice.c
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#include "dds_Mortice.h"

#include <ucdr/microcdr.h>
#include <string.h>



bool MorticeEvent_serialize_topic(ucdrBuffer* writer, const MorticeEvent* topic)
{
    bool success = true;

        success &= TimeStamp_serialize_topic(writer, &topic->stamp);
        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->event);

    return success && !writer->error;
}

bool MorticeEvent_deserialize_topic(ucdrBuffer* reader, MorticeEvent* topic)
{
    bool success = true;

        success &= TimeStamp_deserialize_topic(reader, &topic->stamp);
        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->event);

    return success && !reader->error;
}

uint32_t MorticeEvent_size_of_topic(const MorticeEvent* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += TimeStamp_size_of_topic(&topic->stamp, size);
        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}

bool MorticeRequest_serialize_topic(ucdrBuffer* writer, const MorticeRequest* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->command);

        success &= ucdr_serialize_uint16_t(writer, topic->timeout_ms);

    return success && !writer->error;
}

bool MorticeRequest_deserialize_topic(ucdrBuffer* reader, MorticeRequest* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->command);

        success &= ucdr_deserialize_uint16_t(reader, &topic->timeout_ms);

    return success && !reader->error;
}

uint32_t MorticeRequest_size_of_topic(const MorticeRequest* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 2) + 2;

    return size - previousSize;
}

bool MorticeResponse_serialize_topic(ucdrBuffer* writer, const MorticeResponse* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->command);

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->code);

    return success && !writer->error;
}

bool MorticeResponse_deserialize_topic(ucdrBuffer* reader, MorticeResponse* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->command);

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->code);

    return success && !reader->error;
}

uint32_t MorticeResponse_size_of_topic(const MorticeResponse* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}
