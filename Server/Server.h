#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <errno.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdexcept>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <dirent.h>
#include <vector>

class Server
{

private:
    uint16_t portNumber;
    int serverSocket;
    struct sockaddr_in SocketAddrIn;
    struct sockaddr_in ClientSocketAddrIn;
    fd_set activeFDSet, readFDSet;
    socklen_t clientSize;
    const char *PathToReceiveFolder = "/home/student/Desktop/Documents/Embedded Driven Systems/Assignment 2/Filedropper_Server/";

public:
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
    void ReceiveFile(const char *content, std::string fileName);
    int SearchingForFile(std::string name);
    void SendFile(int descriptor, std::string fileName);
    int GenerateKey();
};

#endif
