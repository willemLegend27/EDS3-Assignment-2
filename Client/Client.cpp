#include <stdexcept>
#include <string>
#include <stdint.h>
#include <iostream>

#include "Client.h"

using namespace std;

Client::Client()
{
}
Client::Client(uint16_t serverPort)
{
    this->serverPort = serverPort;
}

int Client::CreateClientSocket()
{
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
    return 0;
}

int Client::ConnectToServer()
{
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    return 0;
}

void Client::Messaging()
{
    send(clientSocket, messageToSend, strlen(messageToSend), 0);

    valRead = read(clientSocket, messageBuffer, 1024);
}