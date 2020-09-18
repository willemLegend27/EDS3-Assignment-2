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

    while (true)
    {
        //clear the socket set
        FD_ZERO(&readfds);

        //add master socket to set
        FD_SET(masterSocket, &readfds);
        maxSd = masterSocket;

        //add child sockets to set
        for (int i = 0; i < MaxClients; i++)
        {
            //socket descriptor
            sd = clientSockets[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > maxSd)
                maxSd = sd;
        }

        //wait for an activity on one of the sockets , timeout is NULL ,
        //so wait indefinitely
        activity = select(maxSd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            printf("select error");
        }

        //If something happened on the master socket ,
        //then its an incoming connection
        if (FD_ISSET(masterSocket, &readfds))
        {
            if ((newSocket = accept(masterSocket,
                                    (struct sockaddr *)&address, (socklen_t *)&addrLen)) < 0)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }

            //add new socket to array of sockets
            for (int i = 0; i < MaxClients; i++)
            {
                //if position is empty
                if (clientSockets[i] == 0)
                {
                    clientSockets[i] = newSocket;
                    printf("Adding to list of sockets as %d\n", i);

                    break;
                }
            }
        }

        //else its some IO operation on some other socket
        for (int i = 0; i < MaxClients; i++)
        {
            sd = clientSockets[i];

            if (FD_ISSET(sd, &readfds))
            {
                if ((valRead = read(sd, messageBuffer, 1024)) == 0)
                {
                    getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrLen);
                    cout << "Incomming message from client with IP: " << inet_ntoa(address.sin_addr) << " on port: " << ntohs(address.sin_port) << "\n";
                    cout << "Received message : " << messageBuffer << "\n\n";
                    messageBuffer[valRead] = '\0';
                    cout << "Sending acknowledgement to client"
                         << "\n";

                    cout << "Closing socket....\n";
                    close(sd);
                    clientSockets[i] = 0;
                    cout << "Disconnected\n";
                }
                else
                {
                    messageBuffer[valRead] = '\0';
                    send(sd, "ACK", 3, 0);
                }
            }
        }
    }
}