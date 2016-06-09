#include "User.h"


User::User(string username, SOCKET sock)
{

	_username = username;
	_sock = sock;
	_currRoom = NULL;
	_currGame = NULL;
}

User::~User()
{

}


void User::send(string message)
{
	Helper::sendData(_sock, message);
}


Room* User::getRoom()
{
	return _currRoom;
}

void User::setGame(Game* gm)
{
	_currGame = gm;
}

void User::clearRoom()
{
	_currGame = NULL;
}

bool User::createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime)
{
	return false;
	/*

	*/
}

bool User::joinRoom(Room*)
{
	return false;
}

void User::leaveRoom()
{

}

int User::closeRoom()
{
	return 0;
}

bool User::leaveGame()
{
	return false;
}

string User::getUsername()
{
	return _username;
}

SOCKET User::getScoket()
{
	return _sock;
}
