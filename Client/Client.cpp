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

void Client::CreateClientSocket()
{
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serverAddress.sin_addr) <= 0)
    {
        printf("\nInvalid address/ Address not supported \n");
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        printf("\nConnection Failed \n");
    }
    send(clientSocket, messageToSend, strlen(messageToSend), 0);
    printf("Hello message sent\n");
    valRead = read(clientSocket, messageBuffer, 1024);
    printf("%s\n", messageBuffer);
}