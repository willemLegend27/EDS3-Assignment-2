#ifndef Client_h
#define Client_h

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
#include <iostream>

using namespace std;
class Client
{

private:
    int clientSocket = 0;
    int valRead;
    uint16_t serverPort;
    struct sockaddr_in serverAddress;
    char messageBuffer[1050] = {0};
    const char *messageToSend = "Random Message";

public:
    Client();
    ~Client();
    Client(uint16_t serverPort);
    void CreateClientSocket();

private:
};

#endif