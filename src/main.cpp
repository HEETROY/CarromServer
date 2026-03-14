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
#include "includes/network/server.hpp"

#define PORT 3000

int main()
{
    Server server(PORT);
    server.Start();
    int serverSocket = server.GetServerSocket(); // phone number of your server
    // int serverSocket; // phone number of your server
    int clientSocket; // phone call connection to a player

    /* sockaddr_in contains:
     IP address
     Port
     Protocol */
    // struct sockaddr_in serverAddr{}; // server's location
    struct sockaddr_in clientAddr{}; // player's location

    // socklen_t → type used for address size
    socklen_t clientSize = sizeof(clientAddr);

    char buffer[1024];

    // /* Create socket
    //  AF_INET means Use IPv4 internet addresses,
    //  SOCK_STREAM means Use TCP protocol (TCP = reliable connection (like a phone call).)
    //  0 means Use default protocol for TCP */
    // serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // /* If the socket fails to create, it returns negative.
    //  So this checks for failure. */
    // if (serverSocket < 0)
    // {
    //     std::cout << "Socket creation failed\n";
    //     return 1;
    // }

    // std::cout << "Socket created\n";

    // /* Configure server address
    //  Defines: address type = IPv4 */
    // serverAddr.sin_family = AF_INET;

    // /* sin_port = port number.
    //  htons() means: Host To Network Short
    //  It converts numbers to network byte order.
    //  Networking uses big-endian format.*/
    // serverAddr.sin_port = htons(PORT);

    // /* This means: Accept connections from any IP
    //  Example: localhost, 192.168.x.x, internet */
    // serverAddr.sin_addr.s_addr = INADDR_ANY;

    // /* Bind means: Attach the socket to the port. 
    //  Before bind: Socket exists, But has no address 
    //  After bind: Socket listens on port 3000 */
    // if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    // {
    //     std::cout << "Bind failed\n";
    //     return 1;
    // }

    // std::cout << "Bind successful\n";

    // /* Listening 
    //  This tells the OS: Start accepting connections 
    //  10 means: Maximum 10 clients waiting in line. */
    // if (listen(serverSocket, 10) < 0)
    // {
    //     std::cout << "Listen failed\n";
    //     return 1;
    // }

    // std::cout << "Server listening on port " << PORT << std::endl;

    /* Means: Run forever. Game servers never stop. */
    while (true)
    {
        /* Accepting a client 
         This means: Wait for a player to connect.
         When a player connects:
         serverSocket = listening phone
         clientSocket = actual call connection */
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

        if (clientSocket < 0)
        {
            std::cout << "Client connection failed\n";
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        /* Converting client IP 
         This converts the binary IP into text.
         Example result: 127.0.0.1 */
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

        std::cout << "[CONNECT] Client connected from " << clientIP << std::endl;

        /* Packet receive loop. Now we listen for messages from that player. */
        while (true)
        {
            PacketHeader header;

            /* Receiving header
             recv() means:
             Read bytes from the network
             Arguments:
             clientSocket → which player
             &header → where to store bytes
             sizeof(PacketHeader) → how many bytes
             0 → flags */
            int headerBytes = recv(clientSocket, &header, sizeof(PacketHeader), 0);

            /* Network conversion
             ntohs means: Network To Host Short.
             It converts the number back to your computer’s format. */
            header.size = ntohs(header.size);

            std::cout << "[PACKET] Received header -> Size: "
                      << header.size
                      << " Type: "
                      << (int)header.type
                      << std::endl;

            /* Checking disconnect
             If recv() returns:
             0 → client disconnected
             -1 → error */
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