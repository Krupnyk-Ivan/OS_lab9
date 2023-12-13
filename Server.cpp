#include "Server.h"
#include "mainwindow.h"
QString strr;

Server::Server() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        //std::cerr << "WSAStartup failed.\n";
        strr = "WSAStartup failed.\n";
        vec.push_back(strr);
        exit(EXIT_FAILURE);
    }

    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        //std::cerr << "Socket creation failed.\n";
        strr = "Socket creation failed.\n";
        vec.push_back(strr);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(DEFAULT_PORT);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        //std::cerr << "Binding failed.\n";
        strr = "Binding failed.\n";
        vec.push_back(strr);
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        //std::cerr << "Listening failed.\n";
        strr = "Listening failed.\n";
        vec.push_back(strr);
        closesocket(serverSocket);
        WSACleanup();
        exit(EXIT_FAILURE);
    }
    //std::cout << "Server is listening on port " << DEFAULT_PORT << "...\n";
    strr = "Server is listening on port " + QString::number(DEFAULT_PORT) + "...\n";
    vec.push_back(strr);
}

Server::~Server() {
    closesocket(serverSocket);
    WSACleanup();
}

void Server::start() {
    while (true) {
        SOCKET clientSocket;
        struct sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &addrLen)) == INVALID_SOCKET) {
            //std::cerr << "Accepting client failed.\n";
            strr = "Accepting client failed.\n";
            vec.push_back(strr);
            break;
        }

        //std::cout << "Client connected.\n";
        strr = "Client connected.\n";
        vec.push_back(strr);

        std::thread clientThread(&Server::clientHandler, this, clientSocket);
        clientThread.detach();
        clientThreads.push_back(std::move(clientThread));

        if (clientThreads.size() >= 10) {
            //std::cout << "Reached maximum number of clients (10). Closing the server.\n";
            strr = "Reached maximum number of clients (10). Closing the server.\n";
            vec.push_back(strr);
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
            //std::cout << "Client disconnected.\n";
            strr = "Client disconnected.\n";
            vec.push_back(strr);
            break;
        }

        buffer[bytesReceived] = '\0';
        //std::cout << "Received: " << buffer << std::endl;
        strr = "Received: ";
        for(int i=0;i<BUFFER_SIZE;i++)
            strr += buffer[i];
        strr += "\n";
        vec.push_back(strr);
    }
    closesocket(clientSocket);
}
