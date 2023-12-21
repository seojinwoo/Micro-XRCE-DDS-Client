
#include <vector>
#include <cstring>
#include <iomanip>
#include <iostream>

class StreamPacket
{
public:
    uint8_t Flag;
    uint8_t Source;
    uint8_t Remote;
    uint16_t ReceivedCRC;
    uint16_t CalculatedCRC;
    bool isValid = false;
    std::vector<uint8_t> Payload;
    std::vector<uint8_t> Frame;

    static const uint16_t crc16_table[];
    static uint16_t GetCalculatedCRC(std::vector<uint8_t> _Payload);
    uint16_t get_uint16(const std::vector<uint8_t> &data);
    void SetPayload(uint8_t source, uint8_t remote, std::vector<uint8_t> one);
    void SetFrame(std::vector<uint8_t> one);
    std::string ShowAllInfo();
    static std::string ShowPayload(std::vector<uint8_t> one);
};
