#include "protocol.h"

#include <arpa/inet.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <sys/select.h>
#include <unistd.h>

static const int SelectTimeout = 2;

int main(void)
{
    int listenFd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenFd < 0)
    {
        perror("cannot create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in sa;
    memset(&sa, 0, sizeof sa);
    sa.sin_family = AF_INET;
    sa.sin_port = htons(PortNumber);
    sa.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listenFd, (struct sockaddr*)&sa, sizeof sa) < 0)
    {
        perror("bind failed");
        close(listenFd);
        exit(EXIT_FAILURE);
    }

    if (listen(listenFd, 10) < 0)
    {
        perror("listen failed");
        close(listenFd);
        exit(EXIT_FAILURE);
    }

    while (true)
    {        
        fd_set readFds;
        FD_ZERO(&readFds);
        FD_SET(listenFd, &readFds);

        struct timeval timeout;
        timeout.tv_sec = SelectTimeout;
        timeout.tv_usec = 0;

        int nrSockets = select(FD_SETSIZE, &readFds, NULL, NULL, &timeout);

        if (nrSockets < 0) // error situation
        {
            perror("error from calling socket");
        }
        else if (nrSockets == 0) // timeout
        {
            std::cout << "still listening\n";
        }
        else // found activity, find out what kind
        {
            if (FD_ISSET(listenFd, &readFds))
            {
                int communicationFd = accept(listenFd, NULL, NULL);
                if (communicationFd < 0)
                {
                    perror("accept failed");
                    close(listenFd);
                    exit(EXIT_FAILURE);
                }

                const int BufferSize = 100;
                char buffer[BufferSize];
                int nrBytes = read(communicationFd, buffer, BufferSize - 1);
                if (nrBytes >= 0)
                {
                    buffer[nrBytes] = '\0';
                    std::cout << "received " << nrBytes << " bytes: " << buffer << std::endl;
                }

                if (shutdown(communicationFd, SHUT_RDWR) < 0)
                {
                    perror("shutdown failed");
                    close(communicationFd);
                    close(listenFd);
                    exit(EXIT_FAILURE);
                }
                close(communicationFd);
            }
        }
    }

    close(listenFd);
    return EXIT_SUCCESS;
}