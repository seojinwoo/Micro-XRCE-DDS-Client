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
 * @file dds_AIQR.c
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#include "dds_AIQR.h"

#include <ucdr/microcdr.h>
#include <string.h>



bool AIQREvent_serialize_topic(ucdrBuffer* writer, const AIQREvent* topic)
{
    bool success = true;

        success &= TimeStamp_serialize_topic(writer, &topic->stamp);
        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->event);

        success &= ucdr_serialize_string(writer, topic->read_string);

    return success && !writer->error;
}

bool AIQREvent_deserialize_topic(ucdrBuffer* reader, AIQREvent* topic)
{
    bool success = true;

        success &= TimeStamp_deserialize_topic(reader, &topic->stamp);
        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->event);

        success &= ucdr_deserialize_string(reader, topic->read_string, 255);

    return success && !reader->error;
}

uint32_t AIQREvent_size_of_topic(const AIQREvent* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += TimeStamp_size_of_topic(&topic->stamp, size);
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 4) + 4 + (uint32_t)strlen(topic->read_string) + 1;

    return size - previousSize;
}

bool AIQRRequest_serialize_topic(ucdrBuffer* writer, const AIQRRequest* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->command);

    return success && !writer->error;
}

bool AIQRRequest_deserialize_topic(ucdrBuffer* reader, AIQRRequest* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->command);

    return success && !reader->error;
}

uint32_t AIQRRequest_size_of_topic(const AIQRRequest* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}

bool AIQRResponse_serialize_topic(ucdrBuffer* writer, const AIQRResponse* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->command);

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->code);

    return success && !writer->error;
}

bool AIQRResponse_deserialize_topic(ucdrBuffer* reader, AIQRResponse* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->command);

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->code);

    return success && !reader->error;
}

uint32_t AIQRResponse_size_of_topic(const AIQRResponse* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}
