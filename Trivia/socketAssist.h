#pragma once

#include <winSock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <iostream>

#define PORT "8820"
#define GOOD 1
#define BAD 0


class socketAssist
{
public:


	static void initWinsock();
	static void createSocket();
	static void bindServer();
	static int listenClient();
	static SOCKET acceptClient();
	static int recvData(SOCKET ClientSocket);
	static int sendData(SOCKET ClientSocket);
	static int shutdown();

private:
	//static variables
	static WSADATA wsaData;

	static struct addrinfo *result;
	static struct addrinfo hints;
	static SOCKET ListenSocket;
};

