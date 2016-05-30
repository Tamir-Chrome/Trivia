#pragma once
#include "socketAssist.h"

using namespace std;

class User
{
public:
	User(string, SOCKET);
	~User();

	void send(string);
	string getUsername();
	SOCKET getScoket();
	//Room* getRoom();
	//Game* getGame();
	//void setGame(Game*);
	void clearRoom();
	bool createRoom(int, string, int, int, int);
	//bool joinRoom(Room*);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();

private:
	string _username;
	//Room* _currRoom;
	//Game* _currGame;
	SOCKET _sock;
};

