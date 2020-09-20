

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
        cout << "Error creating socket\n";
        return -1;
    }

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(serverPort);

    if (inet_pton(AF_INET, clientAddress, &serverAddress.sin_addr) <= 0)
    {
        cout << "Invalid address\n";
        return -1;
    }
    return 0;
}

int Client::ConnectToServer()
{
    if (connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
    {
        cout << "Connection Failed\n";
        return -1;
    }
    return 0;
}

void Client::SendMessage(string message)
{
    char messageToSend[1000];
    strcpy(messageToSend, message.c_str());
    cout << "Sending message: " << message << " to server\n";
    send(clientSocket, messageToSend, strlen(messageToSend), 0);
    cout << "Message send\n";
}

void Client::ReceiveMessage(string messageBackUp)
{
    valRead = read(clientSocket, messageBuffer, 1024);
    cout << "Received message: " << messageBuffer << "\n";
    if (strcmp(messageBuffer, "ACK") == 0)
    {
        cout << "Received acknowledgement, resuming program...\n";
    }
    else
    {
        cout << "No acknowledgement received, trying to resend te message...\n";
        SendMessage(messageBackUp);
    }
}

char *Client::GetClientAddress()
{
    return inet_ntoa(serverAddress.sin_addr);
}

void Client::Disconnect()
{
    close(clientSocket);
    cout << "Disconnected\n";
}
