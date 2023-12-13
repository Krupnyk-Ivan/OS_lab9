#pragma once

#include <iostream>
#include <windows.h>
#include <thread>
#include <vector>
#include <QString>

#pragma comment(lib, "ws2_32.lib")

#define DEFAULT_PORT 1488
#define BUFFER_SIZE 1024

class Server {
public:
    Server();
    ~Server();
    void start();
    void stop();

private:
    SOCKET serverSocket;
    std::vector<std::thread> clientThreads;

    void clientHandler(SOCKET clientSocket);
};
