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

int Server::GetPortNumber()
{
    return portNumber;
}

int Server::CreateSocket()
{
    serverSocket = socket(PF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0)
    {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    socketAddrIn.sin_family = AF_INET;
    socketAddrIn.sin_port = htons(portNumber);
    socketAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverSocket, (struct sockaddr *)&socketAddrIn, sizeof(socketAddrIn)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return serverSocket;
}

void Server::CreateServer()
{
    serverSocket = CreateSocket();
    if (listen(serverSocket, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    FD_ZERO(&activeFDSet);
    FD_SET(serverSocket, &activeFDSet);

    while (1)
    {
        /* Block until input arrives on one or more active sockets. */
        readFDSet = activeFDSet;
        if (select(FD_SETSIZE, &readFDSet, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (int i = 0; i < FD_SETSIZE; ++i)
            if (FD_ISSET(i, &readFDSet))
            {
                if (i == serverSocket)
                {
                    /* Connection request on original socket. */
                    int newClient;
                    clientSize = sizeof(clientSocketAddrIn);
                    newClient = accept(serverSocket, (struct sockaddr *)&clientSocketAddrIn, &clientSize);
                    if (newClient < 0)
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }
                    //fprintf(stderr,
                    //       "Server: connect from host %s, port %hd.\n",
                    //       inet_ntoa(clientSocketAddrIn.sin_addr),
                    //       ntohs(clientSocketAddrIn.sin_port));

                    FD_SET(newClient, &activeFDSet);
                }
                else
                {
                    /* Data arriving on an already-connected socket. */
                    if (ReadMessage(i) < 0)
                    {
                        close(i);
                        FD_CLR(i, &activeFDSet);
                    }
                }
            }
    }
}

void Server::HostClient()
{
}
void Server::ListenForMessages()
{
}
int Server::ReadMessage(int descriptor)
{
    char *buffer = new char[maxMessageSize];
    int nbytes;

    nbytes = read(descriptor, buffer, maxMessageSize);
    if (nbytes < 0)
    {
        /* Read error. */
        perror("read");
        exit(EXIT_FAILURE);
    }
    else if (nbytes == 0)
        /* End-of-file. */
        return -1;
    else
    {
        /* Data read. */
        fprintf(stderr, "Server: got message: `%s'\n", buffer);
        return 0;
    }
}
