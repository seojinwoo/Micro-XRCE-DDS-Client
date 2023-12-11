#ifndef uxrDDS_H
#define uxrDDS_H

bool InitTrasport(zigbangUXR * uxr, char* ip, char* port);
bool InitSession(zigbangUXR * uxr,  uint32_t key);
void MakeStream(zigbangUXR * uxr);
void CreateEntity(zigbangUXR * uxr, uint16_t id);
void MakeTopic(zigbangUXR* uxr, uint8_t topicId, const char* topicName, const char* dataType);
void MakePublisher(zigbangUXR * uxr);
void MakeSubscriber(zigbangUXR * uxr);
void MakeDataWriter(zigbangUXR * uxr, uint8_t datawriterId, const char* topicName, const char* dataType);
void MakeDataReader(zigbangUXR * uxr, uint8_t datareaderId, const char* topicName, const char* dataType);
bool RegisterEntity(zigbangUXR * uxr, uint8_t id);

#endif 
