#include "global.h"

boost::asio::io_context io;
boost::asio::serial_port serialPort(io);

std::vector<uint8_t> SavedToReturn;
uint8_t LastSource = 0;
uint8_t LastRemote = 0;
uint16_t LastLength = 0;
uint16_t payloadCount = 0;
uint8_t crcCount = 0;

void rx_thread()
{
    // Infinite loop that runs the interval job
    while (true)
    {
        // Read Bytes
        uint8_t buffer;
        boost::asio::read(serialPort, boost::asio::buffer(&buffer, 1));

        RxQueue.push(buffer);
    }
}

std::vector<uint8_t> HandlingSerial(uint8_t one)
{
    std::vector<uint8_t> ReturnValue;
    static int Step = 0;

    // std::cout << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(one) << " ";

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

    // std::cout << "Step: " << Step << std::endl;

    if (Step == 99 || Step == 6)
    {
        SavedToReturn.clear();
        Step = 0;
    }

    return ReturnValue;
}

void SerialTask(char *dev, int index)
{
    serialPort.open(dev);
    serialPort.set_option(boost::asio::serial_port_base::baud_rate(9600));
    serialPort.set_option(boost::asio::serial_port_base::character_size(8));
    serialPort.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serialPort.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serialPort.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2019);

    std::cout << "Serial Open " << dev << std::endl;

    std::thread rx485(rx_thread);

    while (true)
    {
        // Tx Serial Part
        if (TxSerialQueue[0].empty() == false)
        {
            std::vector<uint8_t> toSend;
            TxSerialQueue[0].wait_and_pop(toSend);
            boost::asio::write(serialPort, boost::asio::buffer(toSend));
        }

        // Rx Serial Part
        if (RxQueue.empty() == false)
        {
            uint8_t toSend;
            RxQueue.wait_and_pop(toSend);
            std::vector<uint8_t> one = HandlingSerial(toSend);

            if (one.size() >= 7)
            {
                TxUdpQueue.push(one);
            }
        }
    }
}