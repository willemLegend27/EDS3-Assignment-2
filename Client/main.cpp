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
    cout << "Connecting to server...\n";
    if (client->ConnectToServer() == -1)
    {
        cout << "Couldn't connect to server\n";
    }
    else
    {
        cout << "Succesfully connected to server\n";
    }
}

static void sendMessage(Client *client, string message)
{

    client->SendMessage(message);
}

static void awaitMessage(Client *client, string sendMessage)
{
    cout << "Awaiting message from server\n";
    client->ReceiveMessage(sendMessage);
}

static void disconnect(Client *client)
{
    cout << "Disconnecting client from server...\n";
    string message = "Disconnect";
    sendMessage(client, message);
    awaitMessage(client, message);
    client->Disconnect();
}

static void showMenu(void)
{
    cout << "Select a task from the options below by typing out the corresponding number\n\n";
    cout << "1: Connect to server\n";
    cout << "2: Disconnect from server\n";
    cout << "3: Send message\n";
    cout << "4: Exit application\n";
}

int main(void)
{
    Client *client = new Client(8888);
    cout << "Client is ready for use\n";
    bool quit = false;
    string messageToServer = "Test";
    while (quit == false)
    {
        char userInput = '\0';
        showMenu();
        cin >> userInput;
        cout << "Your choice: " << userInput << "\n";
        cin.ignore();
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
            cout << "Invalid input\n\n";
            break;
        }
    }
}
