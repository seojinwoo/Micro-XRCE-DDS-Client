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
    bool DebugHere = true;
    // Infinite loop that runs the interval job
    zigbang_log(LOG_WARN, "Start Serial RX");

    while (true)
    {
        // Read Bytes
        uint8_t buffer;
        boost::asio::read(serialPort, boost::asio::buffer(&buffer, 1));

        RxQueue.push(buffer);
    }

    zigbang_log(LOG_WARN, "Exit Serial RX");
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
        zigbang_log(LOG_INFO, (static_cast<std::ostringstream &&>(std::ostringstream() << "Step: " << static_cast<int>(Step))).str());
    }

    if (Step == 99 || Step == 6)
    {
        SavedToReturn.clear();
        Step = 0;
    }

    return ReturnValue;
}

void SetSerial(int fd)
{
    struct termios options;
    tcgetattr(fd, &options);

    // Set baud rate
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    /* Setting CONTROL OPTIONS. */
    options.c_cflag |= unsigned(CREAD);    // Enable read.
    options.c_cflag |= unsigned(CLOCAL);   // Set local mode.
    options.c_cflag &= unsigned(~PARENB);  // Disable parity.
    options.c_cflag &= unsigned(~CSTOPB);  // Set one stop bit.
    options.c_cflag &= unsigned(~CSIZE);   // Mask the character size bits.
    options.c_cflag |= unsigned(CS8);      // Set 8 data bits.
    options.c_cflag &= unsigned(~CRTSCTS); // Disable hardware flow control.

    /* Setting LOCAL OPTIONS. */
    options.c_lflag &= unsigned(~ICANON); // Set non-canonical input.
    options.c_lflag &= unsigned(~ECHO);   // Disable echoing of input characters.
    options.c_lflag &= unsigned(~ECHOE);  // Disable echoing the erase character.
    options.c_lflag &= unsigned(~ISIG);   // Disable SIGINTR, SIGSUSP, SIGDSUSP and SIGQUIT signals.

    /* Setting INPUT OPTIONS. */
    options.c_iflag &= unsigned(~IXON);   // Disable output software flow control.
    options.c_iflag &= unsigned(~IXOFF);  // Disable input software flow control.
    options.c_iflag &= unsigned(~INPCK);  // Disable parity check.
    options.c_iflag &= unsigned(~ISTRIP); // Disable strip parity bits.
    options.c_iflag &= unsigned(~IGNBRK); // No ignore break condition.
    options.c_iflag &= unsigned(~IGNCR);  // No ignore carrier return.
    options.c_iflag &= unsigned(~INLCR);  // No map NL to CR.
    options.c_iflag &= unsigned(~ICRNL);  // No map CR to NL.

    /* Setting OUTPUT OPTIONS. */
    options.c_oflag &= unsigned(~OPOST); // Set raw output.

    /* Setting OUTPUT CHARACTERS. */
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 1;

    tcsetattr(fd, TCSANOW, &options);
}

void SerialTask(char *dev, int index)
{
    int fd = open(dev, O_RDWR | O_NOCTTY | O_NONBLOCK);
    SetSerial(fd);

    serialPort.open(dev);
    serialPort.set_option(boost::asio::serial_port_base::baud_rate(115200));
    serialPort.set_option(boost::asio::serial_port_base::character_size(8));
    serialPort.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serialPort.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serialPort.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 2019);

    zigbang_log(LOG_WARN, "Serial Open " + std::string(dev));

    std::thread rx485(rx_thread);

    while (true)
    {
        // Tx Serial Part
        if (TxSerialQueue[0].empty() == false)
        {
            std::vector<uint8_t> toSend;
            TxSerialQueue[0].wait_and_pop(toSend);

            zigbang_log(LOG_WARN, "Client << Serial << Send " + std::to_string(toSend.size()) + " bytes to Serial");

            for (std::size_t i = 0; i < toSend.size(); ++i)
            {
                // Write one byte to the serial port
                boost::asio::write(serialPort, boost::asio::buffer(&toSend[i], 1));

                // Optionally, add a delay between each write
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
            }
        }

        // Rx Serial Part
        if (RxQueue.empty() == false)
        {
            uint8_t toSend;
            RxQueue.wait_and_pop(toSend);
            std::vector<uint8_t> one = HandlingSerial(toSend);

            if (one.size() >= 7)
            {
                zigbang_log(LOG_WARN, "Client >> Serial >> Received " + std::to_string(one.size()) + " bytes to Serial");
                TxUdpQueue.push(one);
            }
        }
    }
}