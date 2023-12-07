#ifndef uxrDDS_H
#define uxrDDS_H

bool InitTrasport(zigbangUXR * uxr, char* ip, char* port);
bool InitSession(zigbangUXR * uxr,  uint32_t key);
void MakeStream(zigbangUXR * uxr);
void CreateEntity(zigbangUXR * uxr);
void MakeTopic(zigbangUXR * uxr);
void MakePublisher(zigbangUXR * uxr);
void MakerDataWriter(zigbangUXR * uxr);
bool RegisterEntity(zigbangUXR * uxr);

#endif 
