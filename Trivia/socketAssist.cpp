#include "socketAssist.h"
#include "Helper.h"
#define PORT 8820
#define IFACE NULL

//link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

//static variables defined in compilitionf
SOCKET socketAssist::_socket;
WSADATA wsaData;

void socketAssist::initWinsock()
{
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		throw exception("WSAStartup failed");
	}
}

void socketAssist::createSocket()
{

	// notice that we step out to the global namespace
	// for the resolution of the function socket
	_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (_socket == INVALID_SOCKET)
	{
		WSACleanup();
		throw std::exception(__FUNCTION__ " - socket");
	}
}

// listen to connecting requests from clients
void socketAssist::bindAndListen()
{
	struct sockaddr_in sa = { 0 };
	sa.sin_port = htons(PORT);
	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = IFACE;
	// again stepping out to the global namespace
	if (::bind(_socket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	if (::listen(_socket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
}

int socketAssist::acceptClient()
{
	SOCKET client_socket = ::accept(_socket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	return client_socket;
}

void socketAssist::shutdown()
{
	::closesocket(_socket);
	WSACleanup();
}