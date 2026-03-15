#include "../includes/network/server.hpp"
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
 These let computers talk to each other.
*/
#include <sys/socket.h>

Server::Server(int port)
    :m_port(port)
{
    std::cout << "SERVER CONSTRUCTED" << std::endl;
}

Server::~Server()
{
    std::cout << "SERVER DESTRUCTED" << std::endl;
}

bool Server::Start()
{
    if(!this->CreateSocket()) return false;
    std::cout << "Socket created" << std::endl;

    this->ConfigureServer();
    std::cout << "Server configured" << std::endl;

    if(!this->BindServer()) return false;
    std::cout << "Server Bind Succesful" << std::endl;

    if(!this->StartListening()) return false;
    std::cout << "Server Listening On Port: " << m_port << std::endl;

    return true;
}

bool Server::CreateSocket()
{
    /* Create socket
     AF_INET means Use IPv4 internet addresses,
     SOCK_STREAM means Use TCP protocol (TCP = reliable connection (like a phone call).)
     0 means Use default protocol for TCP
    */
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    /* If the socket fails to create, it returns negative.
     So this checks for failure. */
    if (serverSocket < 0)
    {
        std::cout << "Socket creation failed\n";
        return false;
    }

    return true;
}

void Server::ConfigureServer()
{
    /* Configure server address
     Defines: address type = IPv4
    */
    serverAddr.sin_family = AF_INET;

    /* sin_port = port number.
     htons() means: Host To Network Short
     It converts numbers to network byte order.
     Networking uses big-endian format.
    */
    serverAddr.sin_port = htons(m_port);

    /* This means: Accept connections from any IP
     Example: localhost, 192.168.x.x, internet
    */
    serverAddr.sin_addr.s_addr = INADDR_ANY;
}

bool Server::BindServer()
{
    /* Bind means: Attach the socket to the port.
     Before bind: Socket exists, But has no address
     After bind: Socket listens on port 3000
    */
    if (bind(serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cout << "Bind failed\n";
        return false;
    }

    return true;
}

bool Server::StartListening()
{
    /* Listening
     This tells the OS: Start accepting connections
     10 means: Maximum 10 clients waiting in line.
    */
    if (listen(serverSocket, 10) < 0)
    {
        std::cout << "Listen failed\n";
        return false;
    }

    return true;
}

int Server::GetServerSocket() { return serverSocket; }

void Server::Run()
{
    while (true)
    {
        // this->AcceptSingleClient();
    }
}

// void Server::AcceptSingleClient()
// {
//     sockaddr_in clientAddr();

//         /* Accepting a client 
//          This means: Wait for a player to connect.
//          When a player connects:
//          serverSocket = listening phone
//          clientSocket = actual call connection 
//         */
//         int clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientSize);

//         if (clientSocket < 0)
//         {
//             std::cout << "Client connection failed\n";
//             return;
//         }

//         char clientIP[INET_ADDRSTRLEN];
//         /* Converting client IP 
//          This converts the binary IP into text.
//          Example result: 127.0.0.1 */
//         inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

//         std::cout << "[CONNECT] Client connected from " << clientIP << std::endl;
// }