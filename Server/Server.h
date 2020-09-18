#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;
class Server
{

private:
    uint16_t portNumber;
    fd_set readfds;
    const int MaxClients = 10;
    int masterSocket;
    int addrLen;
    int newSocket;
    int clientSockets[30];
    int activity;
    int valRead;
    int sd;
    int opt = true;
    int maxSd;
    struct sockaddr_in address;
    char messageBuffer[1500];

public:
    Server();
    Server(uint16_t portNumber);
    virtual ~Server(){};
    uint16_t GetPortNumber() const;
    void ResetClientSockets();
    void SetupServer();
    void CreateMasterSocket();
    void ListenForClients();

private:
};

#endif