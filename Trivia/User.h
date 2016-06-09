#pragma once
#include "socketAssist.h"
#include "Helper.h"




using namespace std;

class Room;
class Game;

class User
{
public:
	User(string username, SOCKET sock);
	~User();

	void send(string);
	std::string getUsername();
	SOCKET getScoket();
	Room* getRoom();
	//Game* getGame();
	void setGame(Game*);
	void clearRoom();
	bool createRoom(int roomId, string roomName, int maxUsers, int questionsNo, int questionTime);
	bool joinRoom(Room*);
	void leaveRoom();
	int closeRoom();
	bool leaveGame();

private:
	string _username;
	Room* _currRoom;
	Game* _currGame;
	SOCKET _sock;
};

