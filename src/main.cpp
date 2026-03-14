#include <iostream>

/* This toolbox contains socket functions.
A socket is like a telephone wire between two computers.
Functions from this toolbox:
socket()
bind()
listen()
accept()
recv()
send()
These let computers talk to each other. */
#include <sys/socket.h>

/* This toolbox contains internet address structures.
It lets you describe things like: 
IP address
Port
Protocol
Example structure used later: sockaddr_in */
#include <netinet/in.h>

/* This toolbox contains operating system commands.
Example: close() Which closes a connection. */
#include <unistd.h>

/* This toolbox helps work with raw memory.
Examples: memcpy, memset. 
You don’t use it heavily yet, but it’s common in networking. */
#include <cstring>

/*This toolbox contains IP address conversion tools.
Example function used in your code: 
inet_ntop()
Which converts a binary IP into something readable like:
127.0.0.1 */
#include <arpa/inet.h>

#include "packet.hpp"
#include "packet_types.hpp"
#include "join_packet.hpp"
#include "strike_packet.hpp"

#define PORT 3000

int main()
{
    int serverSocket; // phone number of your server
    int clientSocket; // phone call connection to a player

    /* sockaddr_in contains:
     IP address
     Port
     Protocol */
    struct sockaddr_in serverAddr{}; // server's location
    struct sockaddr_in clientAddr{}; // player's location

    // socklen_t → type used for address size
    socklen_t clientSize = sizeof(clientAddr);

    char buffer[1024];

    /* Create socket
     AF_INET means Use IPv4 internet addresses,
     SOCK_STREAM means Use TCP protocol
     0 means Use default protocol for TCP */
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

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

        std::cout << "[CONNECT] Client connected from " << clientIP << std::endl;

        while (true)
        {
            PacketHeader header;

            int headerBytes = recv(clientSocket, &header, sizeof(PacketHeader), 0);
            header.size = ntohs(header.size);

            std::cout << "[PACKET] Received header -> Size: "
                      << header.size
                      << " Type: "
                      << (int)header.type
                      << std::endl;

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
                default:
                {
                    std::cout << "[WARNING] Unknown packet type: "
                              << (int)header.type
                              << std::endl;
                }
            }
        }

        
    }

    return 0;
}