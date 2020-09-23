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
    ReceiveMessage();
}

void Client::ReceiveMessage()
{
    const int BufferSize = 1050;
    char messageBuffer[BufferSize] = {0};
    valRead = read(clientSocket, messageBuffer, 1024);
    std::cout << "Received message: " << messageBuffer << "\n";
    if (strcmp(messageBuffer, "ACK") == 0)
    {
        std::cout << "Received acknowledgement, resuming program...\n";
    }
}

void Client::SendFile(std::string fileName, std::string message)
{

    if (SearchingForFile(fileName) == -1)
    {
        std::cout << "Error sending file\n";
    }
    else
    {
        SendMessage(message);
        const int BufferSize = 1050;
        char messageBuffer[BufferSize] = {0};
        messageBuffer[BufferSize] = '\0';
        std::cout << "Sending file to server\n";
        FILE *descriptor = fopen(PathToClientFolder, "rw");
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
}

int Client::SearchingForFile(std::string name)
{
    std::string dir = PathToClientFolder;
    std::vector<std::string> files = std::vector<std::string>();
    DIR *dp;
    struct dirent *dirp;

    if ((dp = opendir(dir.c_str())) == NULL)
    {
        std::cout << "Error opening directory\n";
    }
    while ((dirp = readdir(dp)) != NULL)
    {
        files.push_back(std::string(dirp->d_name));
    }
    closedir(dp);
    for (unsigned int i = 0; i < files.size(); i++)
    {

        std::cout << files[i] << "\n";
        std::cout << name;
        if (files[i] == name)
        {
            std::cout << "Found file: " << files[i].c_str() << "\n";
            return 0;
        }
    }
    std::cout << "File doesn't exist\n";
    return -1;
}

void Client::RequestFile(std::string key)
{
    Messaging(key);
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
