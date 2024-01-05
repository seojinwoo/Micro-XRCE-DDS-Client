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
 * @file dds_AISpeaker.c
 * This source file contains the definition of the described types in the IDL file.
 *
 * This file was generated by the tool gen.
 */

#include "dds_AISpeaker.h"

#include <ucdr/microcdr.h>
#include <string.h>



bool AISpeakerEvent_serialize_topic(ucdrBuffer* writer, const AISpeakerEvent* topic)
{
    bool success = true;

        success &= TimeStamp_serialize_topic(writer, &topic->stamp);
        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->event);

    return success && !writer->error;
}

bool AISpeakerEvent_deserialize_topic(ucdrBuffer* reader, AISpeakerEvent* topic)
{
    bool success = true;

        success &= TimeStamp_deserialize_topic(reader, &topic->stamp);
        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->event);

    return success && !reader->error;
}

uint32_t AISpeakerEvent_size_of_topic(const AISpeakerEvent* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += TimeStamp_size_of_topic(&topic->stamp, size);
        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}

bool AISpeakerRequest_serialize_topic(ucdrBuffer* writer, const AISpeakerRequest* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->command);

        success &= ucdr_serialize_uint16_t(writer, topic->voice_type);

    return success && !writer->error;
}

bool AISpeakerRequest_deserialize_topic(ucdrBuffer* reader, AISpeakerRequest* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->command);

        success &= ucdr_deserialize_uint16_t(reader, &topic->voice_type);

    return success && !reader->error;
}

uint32_t AISpeakerRequest_size_of_topic(const AISpeakerRequest* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 2) + 2;

    return size - previousSize;
}

bool AISpeakerResponse_serialize_topic(ucdrBuffer* writer, const AISpeakerResponse* topic)
{
    bool success = true;

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->command);

        success &= ucdr_serialize_uint32_t(writer, (uint32_t)topic->code);

    return success && !writer->error;
}

bool AISpeakerResponse_deserialize_topic(ucdrBuffer* reader, AISpeakerResponse* topic)
{
    bool success = true;

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->command);

        success &= ucdr_deserialize_uint32_t(reader, (uint32_t)&topic->code);

    return success && !reader->error;
}

uint32_t AISpeakerResponse_size_of_topic(const AISpeakerResponse* topic, uint32_t size)
{
    uint32_t previousSize = size;
        size += ucdr_alignment(size, 4) + 4;

        size += ucdr_alignment(size, 4) + 4;

    return size - previousSize;
}
