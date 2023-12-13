#include "Server.h"
#include <windows.h>

HANDLE clientThreadsMutex;

Server::Server() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        exit(EXIT_FAILURE);
    }
    clientThreadsMutex = CreateMutex(NULL, FALSE, NULL);
    if (clientThreadsMutex == NULL) {
        std::cerr << "Mutex creation failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(DEFAULT_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Binding failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listening failed.\n";
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    std::cout << "Server is listening on port " << DEFAULT_PORT << "...\n";
}

Server::~Server() {
    CloseHandle(clientThreadsMutex);
    closesocket(serverSocket);
    WSACleanup();
}

void Server::start() {
    while (true) {
        SOCKET clientSocket;
        struct sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen)) == INVALID_SOCKET) {
            std::cerr << "Accepting client failed.\n";
            break;
        }

        std::cout << "Client connected.\n";

        WaitForSingleObject(clientThreadsMutex, INFINITE);
        std::thread clientThread(&Server::clientHandler, this, clientSocket);
        ReleaseMutex(clientThreadsMutex);

        clientThread.detach();
        if (clientThreads.size() >= 10) {
            std::cout << "Reached maximum number of clients (10). Closing the server.\n";
            break;
        }
    }
}

void Server::stop() {
    closesocket(serverSocket);
    WSACleanup();
}

void Server::clientHandler(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    while (true) {
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            std::cout << "Client disconnected.\n";
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Received: " << buffer << std::endl;
    }

    WaitForSingleObject(clientThreadsMutex, INFINITE);
    closesocket(clientSocket);
    ReleaseMutex(clientThreadsMutex);
}
