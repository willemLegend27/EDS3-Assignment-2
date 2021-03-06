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
#include <stdexcept>
#include <stdint.h>
#include <fstream>
#include <dirent.h>
#include <vector>

class Client
{

private:
    int clientSocket = 0;
    int valRead;
    uint16_t serverPort;
    struct sockaddr_in ServerAddress;
    const char *ClientAddress = "127.0.0.1";
    const char *PathToClientFolder = "/home/student/Desktop/Documents/Embedded Driven Systems/Assignment 2/Filedropper_Client/";

public:
    Client(uint16_t serverPort);
    ~Client();
    int CreateClientSocket();
    int ConnectToServer();
    void Messaging(std::string message);
    void SendFile(std::string fileName, std::string message);
    void RequestFile(std::string key);
    char *GetClientAddress();
    void Disconnect();

private:
    void SendMessage(std::string message);
    void ReceiveMessage();
    int SearchingForFile(std::string name);
};

#endif