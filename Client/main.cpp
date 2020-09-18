#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "Client.h"

int main(void)
{
    Client *client = new Client(8888);
    client->CreateClientSocket();
}