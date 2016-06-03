
#include "TriviaServer.h"
#include <thread>
#include "Helper.h"
#include "Room.h"


int TriviaServer::_roomIdSequence;

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

void TriviaServer::clientHandler(SOCKET sock)
{

}

User* TriviaServer::handleSignin(RecievedMessage* msg)
{
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];

	if (_db.isUserAndPassMatch(username, password) == false) //if no match
	{
		Helper::sendData(msg->getSock(), "1021");
		return nullptr;
	}

	if (getUserByName(username)) //if user already connected
	{
		Helper::sendData(msg->getSock(), "1022");
		return nullptr;
	}

	User* newUser = new User(username, msg->getSock());
	_connectedUsers.insert(pair<SOCKET, User*>(msg->getSock(), newUser)); //insert new user to connected users
	Helper::sendData(msg->getSock(), "1020");
	return newUser;
}

bool TriviaServer::handleSignup(RecievedMessage* msg)
{
	return false;
}

void TriviaServer::handleSignOut(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return;

	_connectedUsers.erase(user->getScoket());

	//handleCloseRoom(msg);
	//handleLeaveRoom(msg);
	//handleaveGame(msg);
}


/*
return true if succeeded, false if faild
*/
bool TriviaServer::handleCreateRoom(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return false;

	int maxUsers, questionsNo, questionTime;
	vector<string> values = msg->getValues();
	
	string roomName = values[0];
	maxUsers = atoi(values[1].c_str());
	questionsNo = atoi(values[2].c_str());
	questionTime = atoi(values[3].c_str());
	
	bool isCreated = user->createRoom(_roomIdSequence, roomName, maxUsers, questionsNo, questionTime);
	if (!isCreated) //room did
		return false;

	_roomIdSequence++;

	return true; //created room
}


bool TriviaServer::handleCloseRoom(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return false;

	if (user->getRoom() == nullptr) //no room
		return false;

	if (user->closeRoom() != -1) //if failed
		return false;
	
	return true;
}

bool TriviaServer::handleJoinRoom(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return false;

	int roomId = atoi(msg->getValues()[0].c_str());
	Room* room = getRoomByld(roomId);
	if (!room) // no room
	{
		Helper::sendData(user->getScoket(), "1102");
		return false;
	}

	if (user->joinRoom(room) == false)
		return false;

	return true;
	
}

bool TriviaServer::handleLeaveRoom(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return false;
	
	
	Room* room = user->getRoom();
	if (!room) // no room
		return false;

	user->leaveRoom();
	return true;
}

void TriviaServer::handleGetUsersInRoom(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return;

	int roomId = atoi(msg->getValues()[0].c_str());
	Room* room = getRoomByld(roomId);
	if (!room) // no room
	{
		Helper::sendData(user->getScoket(), "1080");
		return;
	}

	string usersListMsg = room->getUsersListMessage();
	Helper::sendData(user->getScoket(), usersListMsg);
}



void TriviaServer::handleGetRooms(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return;

	string sendMsg = "106";

	//add number of rooms
	string numOfRooms = to_string(_rooms.size());

	for (int i = 0; i < 4 - numOfRooms.size(); i++)//add padding zeros
		sendMsg += '0';

	sendMsg += numOfRooms; //add number of rooms

	if (_rooms.size()) //if have rooms
	{
		for (auto it = _rooms.begin(); it != _rooms.end(); ++it)
		{
			//add room id
			string roomId = to_string(it->first);
			for (int i = 0; i < 4 - roomId.size(); i++)//add padding zeros
				sendMsg += '0';

			sendMsg += roomId; //add room id

			string roomName = it->second->getName();

			//add size of the room's name
			string roomNameSize = to_string(roomName.size());
			for (int i = 0; i < 2 - roomNameSize.size(); i++)//add padding zeros
				sendMsg += '0';
			//add room's name size
			sendMsg += roomNameSize;


			sendMsg += roomName; //add room name

		}
	}

	Helper::sendData(user->getScoket(), sendMsg);
}

User* TriviaServer::getUserByName(string)
{
	return nullptr;
}

User* TriviaServer::getUserBySocket(SOCKET)
{
	return nullptr;
}

Room* TriviaServer::getRoomByld(int)
{
	return nullptr;
}

