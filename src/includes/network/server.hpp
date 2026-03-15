#pragma once

/* This toolbox contains internet address structures.
 It lets you describe things like:
 IP address
 Port
 Protocol
 Example structure used later: sockaddr_in
*/
#include <netinet/in.h>

class Server
{
private:
    int m_port;
    int serverSocket;

    /* sockaddr_in contains:
     IP address
     Port
     Protocol
    */
    sockaddr_in serverAddr;

    bool CreateSocket();
    void ConfigureServer();
    bool BindServer();
    bool StartListening();

public:
    Server(int port);
    ~Server();

    bool Start();
    int GetServerSocket();
    void Run();
    // void AcceptSingleClient();
};
