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
    cout << "Sending message: " << message << " to server\n";
    client->Messaging();
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
            cout << "Connecting to server...\n";
            connectToServer(client);
            break;
        case '3':
            cout << "CSending message to server...\n";
            sendMessage(client, "22");
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
