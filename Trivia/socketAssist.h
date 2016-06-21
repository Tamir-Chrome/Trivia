#pragma once

#include <WinSock2.h>
#include <Windows.h>

class socketAssist
{
public:
	static void initWinsock();
	static void createSocket();
	static void bindAndListen();
	static int acceptClient();
	static void shutdown();

private:
	static SOCKET _socket;
};