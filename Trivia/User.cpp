#include "User.h"
#include "Room.h"
#include "Game.h"

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
	if (_currRoom != NULL)
	{
		Helper::sendData(_sock, "1140");
		return false;
	}
	else
	{
		_currRoom = new Room(roomId, this, roomName, maxUsers, questionsNo, questionTime);
		Helper::sendData(_sock, "1141");
		return true;
	}
}

bool User::joinRoom(Room* newRoom)
{
	if (_currRoom != NULL)
	{
		return false;
	}
	else
	{
		return newRoom->joinRoom(this);
	}
}

void User::leaveRoom()
{
	if (_currRoom != NULL)
	{
		_currRoom->leaveRoom(this);
		_currRoom = NULL;
	}
}

int User::closeRoom()
{
	int roomId;
	if (_currRoom == NULL)
	{
		return -1;
	}
	else
	{
		roomId = _currRoom->closeRoom(this);
		if (roomId != -1)
		{
			delete _currRoom;
			_currRoom = NULL;
		}
	}
}

bool User::leaveGame()
{
	bool flag;
	if (_currGame == NULL)
	{
		return false;
	}
	else
	{
		flag = _currGame->leaveGame(this);
		if (flag)
		{
			_currGame = NULL;
		}
		return flag;
	}
}

string User::getUsername()
{
	return _username;
}

SOCKET User::getScoket()
{
	return _sock;
}

Game* User::getGame()
{
	return _currGame;
}
