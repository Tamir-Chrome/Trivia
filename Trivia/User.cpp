#include "User.h"


User::User(string username, SOCKET sock)
{

	_username = username;

	_sock = sock;
}

User::~User()
{

}


void User::send(string)
{

}


Room* User::getRoom()
{
	return _currRoom;
}


void User::clearRoom()
{

}

bool User::createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime)
{
	return false;
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
