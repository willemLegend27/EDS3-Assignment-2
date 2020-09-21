#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/types.h>
#include <unistd.h>

class Client
{

private:
    int clientSocket = 0;
    int valRead;
    uint16_t serverPort;
    struct sockaddr_in ServerAddress;
    char messageBuffer[1050] = {0};
    const char *ClientAddress = "127.0.0.1";

public:
    ~Client();
    Client(uint16_t serverPort);
    int CreateClientSocket();
    int ConnectToServer();
    void SendMessage(std::string message);
    void ReceiveMessage(std::string messageBackUp);
    char *GetClientAddress();
    void Disconnect();

private:
};

#endif