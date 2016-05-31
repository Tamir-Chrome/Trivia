
#include "TriviaServer.h"
#include <thread>

TriviaServer::TriviaServer()
{
	try
	{
		socketAssist::initWinsock();
		socketAssist::createSocket();
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}
	
}


TriviaServer::~TriviaServer()
{
}

void TriviaServer::serve()
{
	try
	{
		bindAndListen();
		while (true)
		{
			accept();
		}
	}
	catch (exception e)
	{
		cout << e.what() << endl;
	}

}

void TriviaServer::bindAndListen()
{
	socketAssist::bindServer();
	socketAssist::listenClient();
}

void TriviaServer::accept()
{
	SOCKET client = socketAssist::acceptClient();	//wait for client
	thread clientT(&TriviaServer::clientHandler, this, client);	//open new thread for new client
	clientT.detach();
}

bool TriviaServer::handleCreateRoom(RecievedMessage* msg)
{

}

void TriviaServer::clientHandler(SOCKET)
{

}