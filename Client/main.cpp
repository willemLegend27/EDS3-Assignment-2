#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "Client.h"

static void connectToServer(Client *client)
{
    client->CreateClientSocket();
    std::cout << "Connecting to server...\n";
    if (client->ConnectToServer() == -1)
    {
        std::cout << "Couldn't connect to server\n";
    }
    else
    {
        std::cout << "Succesfully connected to server\n";
    }
}

static void sendMessage(Client *client, std::string message)
{

    client->SendMessage(message);
}

static void awaitMessage(Client *client, std::string sendMessage)
{
    std::cout << "Awaiting message from server\n";
    client->ReceiveMessage(sendMessage);
}

static void disconnect(Client *client)
{
    std::cout << "Disconnecting client from server...\n";
    std::string message = "Disconnect";
    sendMessage(client, message);
    awaitMessage(client, message);
    client->Disconnect();
}

static void showMenu(void)
{
    std::cout << "Select a task from the options below by typing out the corresponding number\n\n";
    std::cout << "1: Connect to server\n";
    std::cout << "2: Disconnect from server\n";
    std::cout << "3: Send message\n";
    std::cout << "4: Exit application\n";
}

int main(void)
{
    Client *client = new Client(8888);
    std::cout << "Client is ready for use\n";
    bool quit = false;
    std::string messageToServer = "Test";
    while (quit == false)
    {
        char userInput = '\0';
        showMenu();
        std::cin >> userInput;
        std::cout << "Your choice: " << userInput << "\n";
        std::cin.ignore();
        switch (userInput)
        {
        case '1':
            connectToServer(client);
            break;
        case '2':
            disconnect(client);
            break;
        case '3':
            sendMessage(client, messageToServer);
            awaitMessage(client, messageToServer);
            break;
        case '4':
            quit = true;
            break;
        default:
            std::cout << "Invalid input\n\n";
            break;
        }
    }
}
