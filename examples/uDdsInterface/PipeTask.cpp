#include "global.h"

#pragma warning(disable : 4996)

void ProcessStream(char Data);

void PipeTask(int index)
{
    threadRun.set_bit(index);

#ifdef __linux__
    // Create a socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock < 0)
    {
        std::cout << "socket failed: " << strerror(errno) << std::endl;
    }
    int result;

    // make socket's option to resue address
    int optval = 1;
    result = setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if (result < 0)
    {
        std::cout << "setsockopt failed: " << strerror(errno) << std::endl;
        close(sock);
    }

    // make socket's option no use Nagle's algorithm
    optval = 1;
    result = setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, &optval, sizeof(optval));
    if (result < 0)
    {
        std::cout << "setsockopt failed: " << strerror(errno) << std::endl;
        close(sock);
    }

    // make socket's option to get data fast not wait
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    result = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
    if (result < 0)
    {
        std::cout << "setsockopt failed: " << strerror(errno) << std::endl;
        close(sock);
    }

#else
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0)
    {
        std::cout << "WSAStartup failed: " << result << std::endl;
    }

    // Create a socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET)
    {
        std::cout << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
    }
#endif

    // Connect to the server
#ifdef __linux__
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("192.168.1.204");

    // Connect to the server
    result = connect(sock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (result < 0)
    {
        std::cout << "connect failed: " << strerror(errno) << std::endl;
        close(sock);
    }

#else
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // serverAddr.sin_addr.s_addr = inet_addr("192.168.1.246");
    // serverAddr.sin_addr.s_addr = inet_addr("192.168.1.204");

    result = connect(sock, (sockaddr *)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR)
    {
        std::cout << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
    }
#endif

    bool connected = true;

    while (connected)
    {
        bool DoSomething = false;
        if (threadRun.get_bit(EXIT_PROGRAM_ID))
        {
            connected = false;
        }

        // from Sub. to Server
        if (fromSub.count() > 0)
        {
            DoSomething = true;
            std::string ToServer = fromSub.pop().c_str();
#ifdef __linux__
            result = send(sock, ToServer.c_str(), ToServer.size(), 0);
            if (result < 0)
            {
                connected = false;
                std::cout << "send failed: " << strerror(errno) << std::endl;
                close(sock);
            }
#else
            result = send(sock, ToServer.c_str(), ToServer.size(), 0);
            if (result == SOCKET_ERROR)
            {
                connected = false;
                std::cout << "send failed: " << WSAGetLastError() << std::endl;
                closesocket(sock);
                WSACleanup();
            }
#endif
        }

        // from Server to Pub.
        {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(sock, &readSet);

            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;

#ifdef __linux__
            result = select(sock + 1, &readSet, NULL, NULL, &timeout);

            if (result < 0)
            {
                connected = false;
                std::cout << "select failed: " << strerror(errno) << std::endl;
                close(sock);
            }
            else
            {
                threadIdle.set_bit(index);
            }

            if (result > 0)
            {
                DoSomething = true;
                // Receive a response from the server
                char buffer[4096];
                result = recv(sock, buffer, sizeof(buffer), 0);
                if (result <= 0)
                {
                    std::cout << "recv failed: " << strerror(errno) << std::endl;
                    close(sock);
                }
                else
                {
                    std::string ToPush(buffer);
                    // Make Short ToPush with Length 20
                    if (ToPush.size() > 20)
                    {
                        ToPush = ToPush.substr(0, 20);
                    }

                    for (int Index = 0; Index < result; Index++)
                    {
                        ProcessStream(buffer[Index]);
                    }
                }
            }

            if (DoSomething == false)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

#else
            result = select(0, &readSet, NULL, NULL, &timeout);

            if (result == SOCKET_ERROR)
            {
                connected = false;
                std::cout << "select failed: " << WSAGetLastError() << std::endl;
                std::cout << "Check! The Server is waiting" << std::endl;
                closesocket(sock);
                WSACleanup();
            }
            else
            {
                threadIdle.set_bit(index);
            }

            if (result > 0)
            {
                // Receive a response from the server
                char buffer[1024];
                result = recv(sock, buffer, sizeof(buffer), 0);
                if (result == SOCKET_ERROR)
                {
                    std::cout << "recv failed: " << WSAGetLastError() << std::endl;
                    closesocket(sock);
                    WSACleanup();
                }
                else
                {
                    buffer[result] = '\0';
                    std::cout << "Received message: " << buffer << std::endl;

                    std::string ToPush(buffer);
                    toPub.push(ToPush);
                }
            }
            if (DoSomething == false)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
#endif
        }
    }

#ifdef __linux__
    close(sock);
#else
    closesocket(sock);
    WSACleanup();
#endif

    threadRun.clear_bit(index);
    std::cout << "Exit Pipe Thread" << std::endl;
}

void ProcessStream(char Data)
{
    static uint32_t Step = 0;
    static uint32_t Variable = 0;
    static uint32_t Length = 0;
    static std::string Frame;
    static uint32_t ReceivedCount = 0;
    static bool Step0Error = false;

    switch (Step)
    {
    default:
        Step = 0;
        Variable = 0;
        Length = 0;
        Frame = "";
    case 0: // Wait Start Pattern wait 5 times '$'
    {
        if (Data == '&')
        {
            Step0Error = false;
            Variable++;

            if (Variable == 5)
            {
                Step++;
                Variable = 0;
                Length = 0;
            }
        }
        else
        {
            Variable = 0;
            if (Step0Error == false)
            {
                Step0Error = true;
                std::cout << "Step 0 Error: " << Data << std::endl;
            }
        }
        break;
    }

    case 1: // Wait 5 Digit to Parse Length
    {
        // Check Data is One of Number 0 ~ 9
        if (('0' <= Data) && (Data <= '9'))
        {
            Length *= 10;
            Length += (Data - '0');
            Variable++;

            if (Variable == 5)
            {
                Step++;
                Variable = 0;
                Frame = "";
            }
        }
        else
        {
            std::cout << "Step 1 Error" << std::endl;
            Variable = 0;
            Step = 99;
        }
        break;
    }

    case 2: // Get All Data until Length
    {
        Frame += Data;
        Variable++;
        if (Variable >= Length)
        {
            std::cout << "Received message: " << std::to_string(++ReceivedCount) << std::endl;

            toPub.push(Frame);

            Step = 0;
            Variable = 0;
        }
    }
    break;
    }
}
