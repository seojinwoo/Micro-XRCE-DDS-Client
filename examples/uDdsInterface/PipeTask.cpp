#include "global.h"
#pragma warning(disable:4996) 
void PipeTask(int index)
{
    threadRun.set_bit(index);

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

    // Connect to the server
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(1234);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    // serverAddr.sin_addr.s_addr = inet_addr("192.168.1.246");
    // serverAddr.sin_addr.s_addr = inet_addr("192.168.1.204");

    result = connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR)
    {
        std::cout << "connect failed: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
    }

    bool connected = true;

    while (connected) 
    {
        if (threadRun.get_bit(EXIT_PROGRAM_ID))
        {
            connected = false;
        }

        // from Sub. to Server
        if (fromSub.count() > 0) 
        {
            std::string ToServer = fromSub.pop().c_str();            
            result = send(sock, ToServer.c_str(), ToServer.size(), 0);
            if (result == SOCKET_ERROR)
            {
                connected = false;
                std::cout << "send failed: " << WSAGetLastError() << std::endl;
                closesocket(sock);
                WSACleanup();
            }
        }
        else 
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        
        // from Server to Pub.
        {
            fd_set readSet;
            FD_ZERO(&readSet);
            FD_SET(sock, &readSet);
            timeval timeout;
            timeout.tv_sec = 0;
            timeout.tv_usec = 0;

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
        }
    }
    closesocket(sock);
    WSACleanup();

    std::cout << "Exit Pipe Thread" << std::endl;
    threadRun.clear_bit(index);
}