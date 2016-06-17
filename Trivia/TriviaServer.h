
#pragma once
#include "socketAssist.h"
#include <map>
#include <mutex>

#include <queue>
#include "DataBase.h"
#include "RecievedMessage.h"
#include "User.h"
#include "Protocol.h"
#include <condition_variable>
#include "Game.h"

 
using namespace std;

class TriviaServer
{
public:
	TriviaServer();
	~TriviaServer();
	void serve();

private:

	map<SOCKET, User*> _connectedUsers;
	DataBase _db;
	map<int, Room*> _rooms;
	condition_variable cv;
	mutex _mtxRecievedMessages;
	mutex _mtxRoomId;
	queue<RecievedMessage*> _queRcvMessages;
	static int _roomIdSequence;

	void bindAndListen();
	void accept();

	void clientHandler(SOCKET);
	void safeDeleteUser(RecievedMessage*);
	User* handleSignin(RecievedMessage*);
	bool handleSignup(RecievedMessage*);
	void handleSignOut(RecievedMessage*);

	void handleLeaveGame(RecievedMessage*);
	void handleStartGame(RecievedMessage*);
	//void handlePlayerAnswer(RecievedMessage*);

	bool handleCreateRoom(RecievedMessage*);
	bool handleCloseRoom(RecievedMessage*);
	bool handleJoinRoom(RecievedMessage*);
	bool handleLeaveRoom(RecievedMessage*);
	void handleGetUsersInRoom(RecievedMessage*);
	void handleGetRooms(RecievedMessage*);

	//void handleGetBestScores(RecievedMessage*);
	//void handleGetPersonalStatus(RecievedMessage*);

	void handleRecievedMessages();
	void addRecievedMessage(RecievedMessage*);
	RecievedMessage* buildRecieveMessage(SOCKET, int);

	User* getUserByName(string);
	User* getUserBySocket(SOCKET);
	Room* getRoomByld(int);
};

