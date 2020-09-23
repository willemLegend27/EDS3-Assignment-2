#include "Client.h"

Client::Client(uint16_t serverPort)
{
    this->serverPort = serverPort;
}

int Client::CreateClientSocket()
{
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "Error creating socket\n";
        return -1;
    }

    ServerAddress.sin_family = AF_INET;
    ServerAddress.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, ClientAddress, &ServerAddress.sin_addr) <= 0)
    {
        std::cout << "Invalid address\n";
        return -1;
    }
    return 0;
}

int Client::ConnectToServer()
{
    if (connect(clientSocket, (struct sockaddr *)&ServerAddress, sizeof(ServerAddress)) < 0)
    {
        std::cout << "Connection Failed\n";
        return -1;
    }
    return 0;
}

void Client::SendMessage(std::string message)
{
    char messageToSend[1000];
    strcpy(messageToSend, message.c_str());
    std::cout << "Sending message: " << message << " to server\n";
    send(clientSocket, messageToSend, strlen(messageToSend), 0);
    std::cout << "Message send\n";
}

void Client::Messaging(std::string message)
{
    std::cout << "Sending a message to the server\n";
    SendMessage(message);
    std::cout << "Waiting for response from server\n";
    ReceiveMessage(message);
}

void Client::ReceiveMessage(std::string messageBackUp)
{
    valRead = read(clientSocket, messageBuffer, 1024);
    std::cout << "Received message: " << messageBuffer << "\n";
    if (strcmp(messageBuffer, "ACK") == 0)
    {
        std::cout << "Received acknowledgement, resuming program...\n";
    }
    else
    {
        std::cout << "No acknowledgement received, trying to resend te message...\n";
        Messaging(messageBackUp);
    }
}

void Client::SendFile()
{
    std::cout << "Sending file to server\n";
    FILE *descriptor = fopen(PathToSendFile, "rw");
    int readBytes;
    while (!feof(descriptor))
    {
        if ((readBytes = fread(&messageBuffer, 1, sizeof(messageBuffer), descriptor)) > 0)
            send(clientSocket, messageBuffer, readBytes, 0);
        else
        {
            break;
        }
    }
    std::cout << "File has been sended to server\n";
    fclose(descriptor);
}

char *Client::GetClientAddress()
{
    return inet_ntoa(ServerAddress.sin_addr);
}

void Client::Disconnect()
{
    close(clientSocket);
    std::cout << "Disconnected\n";
}
