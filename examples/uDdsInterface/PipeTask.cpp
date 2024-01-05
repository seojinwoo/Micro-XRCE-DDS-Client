#include "global.h"

#pragma warning(disable : 4996)

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
                char buffer[1024];
                result = recv(sock, buffer, sizeof(buffer), 0);
                if (result < 0)
                {
                    std::cout << "recv failed: " << strerror(errno) << std::endl;
                    close(sock);
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
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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
#endif
            if (DoSomething == false)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
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