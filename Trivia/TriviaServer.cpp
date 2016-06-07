#include "TriviaServer.h"
#include <thread>
#include "Helper.h"
#include "Room.h"
#include "Validator.h"


int TriviaServer::_roomIdSequence;

TriviaServer::TriviaServer()
{
	TRACE("Starting...");
	try
	{
		socketAssist::initWinsock();
		socketAssist::createSocket();
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}
	
}


TriviaServer::~TriviaServer()
{
	//delete connected users
	_connectedUsers.clear();

	////delete recevied messages
	while (!_queRcvMessages.empty()) 
		_queRcvMessages.pop();

	socketAssist::shutdown();
}

void TriviaServer::bindAndListen()
{
	socketAssist::bindServer();
	socketAssist::listenClient();
}

void TriviaServer::serve()
{
	try
	{
		bindAndListen();
		TRACE("binded\nlistening (Port = 8820)");
		thread messageThread(&TriviaServer::handleRecievedMessages, this);
		while (true)
		{
			TRACE("accepting client...")
			accept();
		}
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}

}

void TriviaServer::accept()
{
	SOCKET client = socketAssist::acceptClient();	//wait for client
	TRACE("Client accepted. Client socket = %d", (int)client)
	thread clientT(&TriviaServer::clientHandler, this, client);	//open new thread for new client
	clientT.detach();
}


void TriviaServer::clientHandler(SOCKET client)
{
	int code = Helper::getMessageTypeCode(client);
	RecievedMessage* rec;
	while (code != 0 && code != EXIT)
	{
		rec = buildRecieveMessage(client, code);
		addRecievedMessage(rec);
		code = Helper::getMessageTypeCode(client);
	}
	//make end of connection message
}

void TriviaServer::handleRecievedMessages()
{

}

RecievedMessage* TriviaServer::buildRecieveMessage(SOCKET client, int code)
{
	int numBytesUser;
	int numBytesPass;
	int numBytesEmail;
	string s;
	string userName="";
	string pass="";
	string email="";
	vector<string> v;
	RecievedMessage* recM;
	if (code == SIGN_IN)
	{
		//get username size
		numBytesUser = Helper::getIntPartFromSocket(client, 2);

		//get username
		userName = Helper::getStringPartFromSocket(client, numBytesUser);
		v.push_back(userName);

		//get pass size
		numBytesPass = Helper::getIntPartFromSocket(client, 2);

		//get pass
		pass = Helper::getStringPartFromSocket(client, numBytesPass);
		v.push_back(pass);
	}
	else if (code == SIGN_UP)
	{
		//get username size
		numBytesUser = Helper::getIntPartFromSocket(client, 2);

		//get username
		userName = Helper::getStringPartFromSocket(client, numBytesUser);
		v.push_back(userName);
		
		//get password size
		numBytesPass = Helper::getIntPartFromSocket(client, 2);

		//get password
		pass = Helper::getStringPartFromSocket(client, numBytesPass);
		v.push_back(pass);

		//get email size
		numBytesEmail = Helper::getIntPartFromSocket(client, 2);

		//get email
		email = Helper::getStringPartFromSocket(client, numBytesEmail);
		v.push_back(email);
	}
	else if (code == GET_USERS_ROOM || code == JOIN_ROOM)
	{
		//get room id
		string roomID = Helper::getStringPartFromSocket(client, 4);
		v.push_back(roomID);
	}
	else if (code == CREATE_ROOM)
	{
		//get roomName size
		int numBytesRName = Helper::getIntPartFromSocket(client, 2);

		//get room name
		string roomName = Helper::getStringPartFromSocket(client, numBytesRName);
		v.push_back(roomName);

		//get max players
		string playersNumber = Helper::getStringPartFromSocket(client, 1);
		v.push_back(playersNumber);

		//get number of questions
		string questionsNumber = Helper::getStringPartFromSocket(client, 2);
		v.push_back(questionsNumber);

		//get time for question
		string questionsTimeInSec = Helper::getStringPartFromSocket(client, 2);
		v.push_back(questionsTimeInSec);
	}
	recM = new RecievedMessage(client, code, v);
	return recM;
}

void TriviaServer::addRecievedMessage(RecievedMessage* rec)
{
	//lock scope
	{
		lock_guard<mutex> lock(_mtxRecievedMessages);
		//locked
		_queRcvMessages.push(rec);
		//auto unlocked, out of scope
	}

	//notify handleRecievedMessage
	cv.notify_one();
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
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	string email = msg->getValues()[2];

	if (Validator::isPasswordValid(password) == false)
	{
		Helper::sendData(msg->getSock(), "1041");
		return false;
	}

	if (Validator::isUsernameValid(username) == false)
	{
		Helper::sendData(msg->getSock(), "1043");
		return false;
	}

	if (_db.isUserExists(username))
	{
		Helper::sendData(msg->getSock(), "1042");
		return false;
	}

	if (_db.addNewUser(username, password, email) == false)
	{
		Helper::sendData(msg->getSock(), "1044");
		return false;
	}


	Helper::sendData(msg->getSock(), "1040");
	return true;
}

void TriviaServer::handleSignOut(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return;

	_connectedUsers.erase(user->getScoket());

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
	
	
	unique_lock<mutex> idLock(_mtxRoomId);
	int roomId = _roomIdSequence;
	_roomIdSequence++;
	idLock.unlock();

	
	bool isCreated = user->createRoom(_roomIdSequence, roomName, maxUsers, questionsNo, questionTime);
	if (!isCreated) //room did not create
		return false;


	

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

