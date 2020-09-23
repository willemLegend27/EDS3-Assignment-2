#include "Server.h"

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

    SocketAddrIn.sin_family = AF_INET;
    SocketAddrIn.sin_port = htons(portNumber);
    SocketAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(serverSocket, (struct sockaddr *)&SocketAddrIn, sizeof(SocketAddrIn)) < 0)
    {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    return serverSocket;
}

void Server::CreateServer()
{
    serverSocket = CreateSocket();
    std::cout << "Server has IP: " << GetServerAddress() << "\n";
    if (listen(serverSocket, 1) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    FD_ZERO(&activeFDSet);
    FD_SET(serverSocket, &activeFDSet);
    std::cout << "Awaiting client(s) to connect\n";
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
    clientSize = sizeof(ClientSocketAddrIn);
    int newClient = accept(serverSocket, (struct sockaddr *)&ClientSocketAddrIn, &clientSize);
    if (newClient < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    std::cout << "Client with address:" << inet_ntoa(ClientSocketAddrIn.sin_addr) << " connected to server from port: " << ntohs(ClientSocketAddrIn.sin_port) << "\n";
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
    const int BufferSize = 1050;
    char messageBuffer[BufferSize];
    int nrOfBytes = read(descriptor, messageBuffer, BufferSize);
    if (nrOfBytes < 0)
    {
        perror("read");
        exit(EXIT_FAILURE);
    }
    else if (nrOfBytes == 0)
        return -1;
    else
    {
        std::cout << "Server received message of " << nrOfBytes << " bytes from client with address: " << inet_ntoa(ClientSocketAddrIn.sin_addr) << "\n";
        std::cout << "Received message: " << messageBuffer << "\n";
        std::cout << "Sending acknowlegdement back to client\n";
        char const *Answer = "ACK";
        SendMessage(descriptor, Answer);
        if (receivingFile == true)
        {
            receivingFile = false;
            std::string fileName = GenerateKey();
            ReceiveFile(messageBuffer, fileName);
            std::cout << "Sending key to client";
            SendMessage(descriptor, ("Key for file: " + fileName).c_str());
        }
        if (strcmp(messageBuffer, "Disconnect") == 0)
        {
            CloseClientSocket(descriptor);
        }
        else if (strcmp(messageBuffer, "Send_File") == 0)
        {
            receivingFile = true;
        }
        return 0;
    }
}

std::string Server::GenerateKey()
{
    std::string key = "";
    srand(time(NULL));
    for (int i = 0; i < 4; i++)
    {
        key += std::to_string(rand() % 1000);
    }
    return key;
}

void Server::ReceiveFile(const char *content, std::string fileName)
{

    std::cout << "Creating file: " << PathToReceiveFolder << "\n";
    std::ofstream createdFile(PathToReceiveFolder + fileName);
    createdFile << content;
    std::cout << "Parsed received content to file"
              << "\n";
}

void Server::SendMessage(int desciptor, const char *message)
{
    send(desciptor, message, strlen(message), 0);
}

char *Server::GetServerAddress()
{
    return inet_ntoa(SocketAddrIn.sin_addr);
}

void Server::CloseClientSocket(int descriptor)
{
    std::cout << "Closing connection to client " << descriptor << " .....\n";
    close(descriptor);
    std::cout << "Closed connection to client " << descriptor << " .....\n";
}
