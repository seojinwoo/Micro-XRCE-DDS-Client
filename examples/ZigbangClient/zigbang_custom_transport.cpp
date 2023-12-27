
#include "global.h"

static int fdSerial = -1;

bool KeepRun = true;
std::vector<uint8_t> SavedToReturn;
std::vector<uint8_t> HandlingSerial(uint8_t one);
uint8_t LastSource = 0;
uint8_t LastRemote = 1;
uint16_t LastLength = 0;
uint16_t payloadCount = 0;
uint8_t crcCount = 0;
void rx_thread();
void tx_thread();

std::thread rx485(rx_thread);
std::thread tx485(tx_thread);

extern void zigbang_log(LogLevel level, const std::string &message);

bool InitCustomTransport()
{
    bool rv;
    fdSerial = open("/dev/ttyUSB1", O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fdSerial == -1)
    {
        zigbang_log(LOG_FATAL, "open_port: Unable to open /dev/ttyUSB1 - ");
        rv = false;
    }
    else
    {

        rv = true;
    }

    return rv;
}

void DeInitCustomTransport()
{
}

bool zigbang_custom_transport_open(
    uxrCustomTransport *transport)
{
    (void)transport;

    zigbang_log(LOG_INFO, "Micro XRCE-DDS Client Custom transport: opening");

    RxQueue.clear();
    TxQueue.clear();

    bool rv = (fdSerial != -1);

    return rv;
}

bool zigbang_custom_transport_close(
    uxrCustomTransport *transport)
{
    (void)transport;

    zigbang_log(LOG_INFO, "Micro XRCE-DDS Client Custom transport: closing");

    return (-1 == fdSerial) ? true : (0 == close(fdSerial));
}

size_t zigbang_custom_transport_write(
    uxrCustomTransport *transport,
    const uint8_t *buf,
    size_t len,
    uint8_t *errcode)
{
    (void)transport;
    bool DebugHere = false;

    std::vector<uint8_t> payload(buf, buf + len);

    // Make Header
    uint8_t Length[2];
    Length[0] = (uint8_t)((len >> 0) & 0xFF);
    Length[1] = (uint8_t)((len >> 8) & 0xFF);

    size_t rv = 0;
    uint8_t header[5] = {UXR_FRAMING_BEGIN_FLAG, 0, 1, Length[0], Length[1]};

    payload.insert(payload.begin(), header, header + 5);

    // Make Tail
    uint16_t CrcValue;
    uint8_t CRC[2];
    CrcValue = StreamPacket::GetCalculatedCRC(payload);
    CRC[0] = (uint8_t)((CrcValue >> 0) & 0xFF);
    CRC[1] = (uint8_t)((CrcValue >> 8) & 0xFF);

    payload.insert(payload.end(), CRC, CRC + 2);

    if (DebugHere)
    {
        zigbang_log(LOG_INFO, ">> " + StreamPacket::ShowPayload(payload));
    }
    TxQueue.push(payload);

    // Return to uDDS
    rv = (size_t)len;
    *errcode = 0;

    zigbang_log(LOG_INFO, "Micro XRCE-DDS Client Custom transport: wrote " + std::to_string(rv) + " B");

    return rv;
}

size_t zigbang_custom_transport_read(
    uxrCustomTransport *transport,
    uint8_t *buf,
    size_t len,
    int timeout,
    uint8_t *errcode)
{
    (void)transport;

    size_t rv = 0;

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    if (RxQueue.empty() == false)
    {
        std::vector<uint8_t> one;
        RxQueue.wait_and_pop(one);

        one.erase(one.begin(), one.begin() + 5);
        one.erase(one.end() - 2, one.end());

        std::copy(one.begin(), one.end(), buf);

        rv = one.size();
    }

    *errcode = 0;

    if (rv != 0)
    {
        zigbang_log(LOG_INFO, "Micro XRCE-DDS Client Custom transport: read " + std::to_string(rv) + " B");
    }

    return rv;
}

std::vector<uint8_t> HandlingSerial(uint8_t one)
{
    static auto lastTime = std::chrono::high_resolution_clock::now(); // Initialize to current time
    bool DebugHere = false;
    std::vector<uint8_t> ReturnValue;
    static int Step = 0;

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count();

    if (duration > 5)
    {
        SavedToReturn.clear();
        Step = 0;
    }

    lastTime = currentTime; // Update lastTime to current time

    if (DebugHere)
    {
        zigbang_log(LOG_INFO, (static_cast<std::ostringstream &&>(std::ostringstream() << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(one))).str());
    }

    SavedToReturn.push_back(one);
    switch (Step)
    {
    case 0: // get flag
    {
        if (one == UXR_FRAMING_BEGIN_FLAG ||
            one == UXR_FRAMING_ESC_FLAG)
        {
            Step++;
        }
        else
        {
            SavedToReturn.clear();
        }
    }
    break;

    case 1: // get source
    {
        LastSource = one;
        Step++;
    }
    break;

    case 2: // get remote
    {
        LastRemote = one;
        Step++;
    }
    break;

    case 3: // get Length
    {
        if (SavedToReturn.size() == 4)
        {
            LastLength = one;
        }
        else
        {
            LastLength += one * 0x100;
            payloadCount = 0;
            Step++;
        }
    }
    break;

    case 4: // get payload
    {
        payloadCount++;
        if (payloadCount >= LastLength)
        {
            crcCount = 0;
            Step++;
        }
    }
    break;

    case 5: // get CRC
    {
        crcCount++;
        if (crcCount >= 2)
        {
            ReturnValue = SavedToReturn;
            Step++;
        }
    }
    break;

    default:
    {
        Step = 99;
        break;
    }
    }

    if (DebugHere)
    {
        std::cout << "Step: " << Step << std::endl;
        zigbang_log(LOG_INFO, (static_cast<std::ostringstream &&>(std::ostringstream() << "Step: " << static_cast<int>(Step))).str());
    }

    if (Step == 99 || Step == 6)
    {
        SavedToReturn.clear();
        Step = 0;
    }

    return ReturnValue;
}

void rx_thread()
{
    bool DebugHere = false;
    // Infinite loop that runs the interval job
    zigbang_log(LOG_WARN, "Start Serial RX");
    while (true)
    {
        uint8_t buffer;
        memset(&buffer, '\0', 1);
        int n = read(fdSerial, &buffer, 1);
        if (n > 0)
        {
            if (DebugHere)
            {
                zigbang_log(LOG_INFO, (static_cast<std::ostringstream &&>(std::ostringstream() << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(buffer))).str());
            }

            std::vector<uint8_t> Result = HandlingSerial(buffer);
            if (Result.size() > 0)
            {
                if (DebugHere)
                {
                    zigbang_log(LOG_INFO, "<< " + StreamPacket::ShowPayload(Result));
                }

                RxQueue.push(Result);
            }
        }
    }
    zigbang_log(LOG_WARN, "Exit Serial RX");
}

void tx_thread()
{
    bool DebugHere = true;
    // Infinite loop that runs the interval job
    zigbang_log(LOG_WARN, "Start Serial TX");

    while (true)
    {
        std::vector<u_int8_t> newOne;
        TxQueue.wait_and_pop(newOne);

        int n = write(fdSerial, newOne.data(), newOne.size());
    }

    zigbang_log(LOG_WARN, "Exit Serial TX");
}
