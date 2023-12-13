#include "Client.h"

void Client::connectToServer(const std::string& serverAddress, int port) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed.\n";
        return;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed.\n";
        WSACleanup();
        return;
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    inet_pton(AF_INET, serverAddress.c_str(), &serverAddr.sin_addr);
    serverAddr.sin_port = htons(port);

    
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Socket connection failed.\n";
            closesocket(clientSocket);
            WSACleanup();
            return;
        }
    
 
}

void Client::sendMessage(const std::string& message) {
    std::string fullMessage = selectedMethod + username + ": " + message;
    send(clientSocket, fullMessage.c_str(), fullMessage.length(), 0);
}

void Client::setUsername(const std::string& name) {
    username = name;
}

void Client::chooseConnectionMethod(int method) {
    if (method == 1) {
        selectedMethod = "Socket Connection";
    }
    else if (method == 2) {
        selectedMethod = "Pipe Connection";
    }
}

void Client::closeConnection() {
    closesocket(clientSocket);
    WSACleanup();
}
