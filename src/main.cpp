#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#include "packet.hpp"
#include "packet_types.hpp"
#include "join_packet.hpp"
#include "strike_packet.hpp"

#define PORT 3000

int main()
{
    int serverSocket;
    int clientSocket;

    struct sockaddr_in serverAddr{};
    struct sockaddr_in clientAddr{};
    socklen_t clientSize = sizeof(clientAddr);

    char buffer[1024];

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket < 0)
    {
        std::cout << "Socket creation failed\n";
        return 1;
    }

    std::cout << "Socket created\n";

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Bind failed\n";
        return 1;
    }

    std::cout << "Bind successful\n";

    // Listen
    if (listen(serverSocket, 10) < 0)
    {
        std::cout << "Listen failed\n";
        return 1;
    }

    std::cout << "Server listening on port " << PORT << std::endl;

    while (true)
    {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

        if (clientSocket < 0)
        {
            std::cout << "Client connection failed\n";
            continue;
        }

        std::cout << "Client connected\n";

        /* Connect single, Receive single msg and close connection start */
//        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//
//        if (bytesReceived > 0)
//        {
//            std::cout << "Received " << bytesReceived << " bytes\n";
//        }
//
//        close(clientSocket);
        /* Connect single, Receive single msg and close connection close */
        
        /* Connect single, keep receiving and close connection on client leave start */
//        while (true)
//        {
//            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
//
//            if (bytesReceived <= 0)
//            {
//                std::cout << "Client disconnected\n";
//                break;
//            }
//
//            std::cout << "Received " << bytesReceived << " bytes\n";
//        }
        /* Connect single, keep receiving and close connection on client leave close */
        
        /* Connect Single, Keep Receving and packet support start */
//        while (true)
//        {
//            uint16_t packetSize;
//
//            int sizeBytes = recv(clientSocket, &packetSize, sizeof(packetSize), 0);
//
//            if (sizeBytes <= 0)
//            {
//                std::cout << "Client disconnected\n";
//                break;
//            }
//
//            packetSize = ntohs(packetSize);
//
//            char packetBuffer[1024];
//
//            int bytesReceived = recv(clientSocket, packetBuffer, packetSize, 0);
//
//            if (bytesReceived <= 0)
//            {
//                std::cout << "Packet read failed\n";
//                break;
//            }
//
//            std::cout << "Received packet of size: " << packetSize << std::endl;
//
//            uint8_t packetType = packetBuffer[0];
//
//            std::cout << "Packet type: " << (int)packetType << std::endl;
//        }
        /* Connect Single, Keep Receving and packet support close */
        
        /* Connect Single, Keep Receving and packet reading support start */
        while (true)
        {
            PacketHeader header;

            int headerBytes = recv(clientSocket, &header, sizeof(PacketHeader), 0);

            if (headerBytes <= 0)
            {
                std::cout << "Client disconnected\n";
                break;
            }

            int bodySize = header.size - sizeof(PacketHeader);

            char buffer[1024];

            int bodyBytes = recv(clientSocket, buffer, bodySize, 0);

            if (bodyBytes <= 0)
            {
                std::cout << "Packet read failed\n";
                break;
            }

            switch(header.type)
            {
                case PACKET_JOIN:
                {
                    std::cout << "Player joined\n";
                    break;
                }

                case PACKET_STRIKE:
                {
                    StrikePacket* strike = (StrikePacket*)buffer;

                    std::cout << "Strike received\n";
                    std::cout << "Angle: " << strike->angle << std::endl;
                    std::cout << "Power: " << strike->power << std::endl;
                    break;
                }
            }
        }
        /* Connect Single, Keep Receving and packet reading support close */
        
        
    }

    return 0;
}
