#include <stdexcept>
#include <string>
#include <stdint.h>
#include <iostream>

#include "Server.h"

using namespace std;
Server::Server()
{
}

Server::Server(uint16_t portNumber)
{
    this->portNumber = portNumber;
}

uint16_t Server::GetPortNumber() const
{
    return portNumber;
}

void Server::ResetClientSockets()
{
    for (int i = 0; i < MaxClients; i++)
    {
        clientSockets[i] = 0;
    }
}

void Server::SetupServer()
{
    CreateMasterSocket();
    cout << "Created a server on port: " << portNumber << "\n";
    ListenForClients();
    cout << "Listening for client on port: " << portNumber << "\n";
}

void Server::CreateMasterSocket()
{
    if ((masterSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(portNumber);
}

void Server::ListenForClients()
{
    if (bind(masterSocket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(masterSocket, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //accept the incoming connection
    addrLen = sizeof(address);
    puts("Waiting for connections ...");
}