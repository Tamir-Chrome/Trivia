//#pragma once
//#include "socketAssist.h"
//#include <map>
//#include <mutex>
//#include <queue>
//using namespace std;
//
//class TriviaServer
//{
//public:
//	TriviaServer();
//	~TriviaServer();
//	void server();
//
//private:
//	SOCKET _socket;
//	map<SOCKET, User*> _connectedUsers;
//	//DataBase _db;
//	map<int, Room*> _rooms;
//	mutex _mtxRecievedMessages;
//	queue<RecievedMessage*> _queRcvMessages;
//	static int _roomIdSequence;
//
//	void bindAndListen();
//	void accept();
//	void clientHandler(SOCKET);
//	void safeDeleteUser(RecievedMessage*);
//	User* handleSignin(RecievedMessage*);
//	bool handleSignup(RecievedMessage*);
//	void handleSignOut(RecievedMessage*);
//
//	void handleLeaveGame(RecievedMessage*);
//	void handleStartGame(RecievedMessage*);
//	void handlePlayerAnswer(RecievedMessage*);
//
//	bool handleCreateRoom(RecievedMessage*);
//	bool handleCloseRoom(RecievedMessage*);
//	bool handleJoinRoom(RecievedMessage*);
//	bool handleLeaveRoom(RecievedMessage*);
//	void handleGetUsersInRoom(RecievedMessage*);
//	void handleGetRooms(RecievedMessage*);
//
//	void handleGetBestScores(RecievedMessage*);
//	void handleGetPersonalStatus(RecievedMessage*);
//
//	void handleRecievedMessages();
//	void addRecievedMessage(RecievedMessage*);
//	RecievedMessage* buildRecieveMessage(SOCKET, int);
//
//	User* getUserByName(string);
//	User* getUserBySocket(SOCKET);
//	Room* getRoomByld(int);
//};
