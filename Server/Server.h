#ifndef Server_h
#define Server_h

#include <stdio.h>
#include <stdexcept>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

using namespace std;
class Server
{

private:
    uint16_t portNumber;
    int serverSocket;
    struct sockaddr_in socketAddrIn;
    struct sockaddr_in clientSocketAddrIn;
    fd_set activeFDSet, readFDSet;
    socklen_t clientSize;
    int maxMessageSize = 1000;

public:
    Server();
    Server(uint16_t portNumber);
    virtual ~Server(){};
    int GetPortNumber();
    char *GetServerAddress();
    void CreateServer();

private:
    int CreateSocket();
    void HostClient();
    void ListenForMessages(int descriptor);
    int ReadMessage(int descriptor);
    void SendMessage(int descriptor, const char *message);
    void CloseClientSocket(int descriptor);
};

#endif