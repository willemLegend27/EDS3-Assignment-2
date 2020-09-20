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
    cout << "Server has IP: " << GetServerAddress() << "\n";
    if (listen(serverSocket, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    FD_ZERO(&activeFDSet);
    FD_SET(serverSocket, &activeFDSet);
    cout << "Awaiting client(s) to connect\n";
    while (true)
    {

        readFDSet = activeFDSet;
        if (select(FD_SETSIZE, &readFDSet, NULL, NULL, NULL) < 0)
        {
            perror("select");
            exit(EXIT_FAILURE);
        }

        for (int i = 0; i < FD_SETSIZE; ++i)
        {
            if (FD_ISSET(i, &readFDSet))
            {
                if (i == serverSocket)
                {
                    HostClient();
                }
                else
                {
                    ListenForMessages(i);
                }
            }
        }
    }
}

void Server::HostClient()
{
    clientSize = sizeof(clientSocketAddrIn);
    int newClient = accept(serverSocket, (struct sockaddr *)&clientSocketAddrIn, &clientSize);
    if (newClient < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    cout << "Client with address:" << inet_ntoa(clientSocketAddrIn.sin_addr) << " connected to server from port: " << ntohs(clientSocketAddrIn.sin_port) << "\n";
    FD_SET(newClient, &activeFDSet);
}
void Server::ListenForMessages(int descriptor)
{
    if (ReadMessage(descriptor) < 0)
    {
        close(descriptor);
        FD_CLR(descriptor, &activeFDSet);
    }
}
int Server::ReadMessage(int descriptor)
{
    char *messageBuffer = new char[maxMessageSize];
    int nrOfBytes = read(descriptor, messageBuffer, maxMessageSize);
    if (nrOfBytes < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    else if (nrOfBytes == 0)
        return -1;
    else
    {
        cout << "Server received message: " << messageBuffer << "\n";
        cout << "Sending acknowlegdement back to client\n";
        char const *answer = "ACK";
        SendMessage(descriptor, answer);
        if (strcmp(messageBuffer, "Disconnect") == 0)
        {
            CloseClientSocket(descriptor);
        }
        return 0;
    }
}

void Server::SendMessage(int desciptor, const char *message)
{
    send(desciptor, message, strlen(message), 0);
}

char *Server::GetServerAddress()
{
    return inet_ntoa(socketAddrIn.sin_addr);
}

void Server::CloseClientSocket(int descriptor)
{
    cout << "Closing connection to client " << descriptor << " .....\n";
    close(descriptor);
    cout << "Closed connection to client " << descriptor << " .....\n";
}
