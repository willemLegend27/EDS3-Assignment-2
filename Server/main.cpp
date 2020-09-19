#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "Server.h"

int main(void)
{
    Server *server = new Server(8888);
    cout << "Created server on port: " << server->GetPortNumber() << "\n";
    server->CreateServer();
}