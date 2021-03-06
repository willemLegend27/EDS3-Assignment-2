#include "protocol.h"

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

int main(void)
{
    int socketFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (socketFd == -1)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PortNumber);

    int result = inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr);
    if (result != 1)
    {
        perror("could not convert ip address to network address structure");
        exit(EXIT_FAILURE);
    }
    else
    {
        result = connect(socketFd, (struct sockaddr*)&sa, sizeof sa);

        if (result < 0)
        {
            perror("connect failed");
            close(socketFd);
            exit(EXIT_FAILURE);
        }

        std::string text = "Hello World!";
        size_t nrBytes = send(socketFd, text.c_str(), text.length(), 0);
        if (nrBytes != text.length())
        {
            std::cout << "not everything is sent (" << nrBytes << "/" << text.length() << " bytes sent)\n";
        }

        if (shutdown(socketFd, SHUT_RDWR) < 0)
        {
            perror("shutdown failed");
            close(socketFd);
            exit(EXIT_FAILURE);
        }

        close(socketFd);
        return EXIT_SUCCESS;
    }

    return EXIT_FAILURE;
}