#pragma once

#include <iostream>
//#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

class Client {
public:
    void connectToServer(const std::string& serverAddress, int port);
    void sendMessage(const std::string& message);
    void setUsername(const std::string& name);
    void chooseConnectionMethod(int method);
    void closeConnection();

private:
    SOCKET clientSocket;
    std::string username;
    std::string selectedMethod;
};
