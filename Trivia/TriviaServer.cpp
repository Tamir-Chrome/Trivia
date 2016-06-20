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
		TRACE("Exception was catch in c'tor TriviaServer. what= %s\n", e.what());
		system("PAUSE");
		exit(1);
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
	int code;
	RecievedMessage* rec;
	try
	{

		while (true)
		{
			code = Helper::getMessageTypeCode(client);
			rec = buildRecieveMessage(client, code);
			addRecievedMessage(rec);
		}
		//make end of connection message
	}
	catch (exception e)
	{
		TRACE("Exception was catch in function clientHandler. socket=%d, what=%s\n", client, e.what());
	}
}

void TriviaServer::handleRecievedMessages()
{
	unique_lock<mutex> rcvMsgLock(_mtxRecievedMessages);
	cv.wait(rcvMsgLock);

	RecievedMessage* msg = _queRcvMessages.front();
	_queRcvMessages.pop();

	rcvMsgLock.unlock();

	try
	{
		bool useless_flag = true;
		msg->setUser(getUserBySocket(msg->getSock()));
		int code = msg->getMessageCode();
		int sock = msg->getSock();

		TRACE("--------------------------------------");
		TRACE("handleRecievedMessages: msgCode = %d, client_socket: %d", code, sock);
		User* user = nullptr;
		switch (code)
		{

		case SIGN_IN:
			user = handleSignin(msg);
			if (user)
			{
				TRACE("SEND: User signed in successfuly: username = %s, socket = %d", user->getUsername().c_str(), sock)
			}
			else
				TRACE("SEND: User try sign in with wrong details: username = %s, socket = %d", msg->getValues()[0].c_str(), sock)
			break;

		case SIGN_OUT:
			handleSignOut(msg);
			break;

		case SIGN_UP:
			useless_flag = handleSignup(msg);
			break;

		case GET_ROOMS:
			handleGetRooms(msg);
			break;

		case GET_USERS_ROOM:
			handleGetUsersInRoom(msg);
			break;

		case JOIN_ROOM:
			//wtf am i suppose to do with useless_flag !!?
			useless_flag = handleJoinRoom(msg);
			break;

		case LEAVE_ROOM:
			//wtf am i suppose to do with useless_flag !!?
			useless_flag = handleLeaveRoom(msg);
			break;

		case CREATE_ROOM:
			useless_flag = handleLeaveRoom(msg);
			break;

		case CLOSE_ROOM:
			useless_flag = handleCloseRoom(msg);
			break;

		default:
			TRACE("EXIT")
			safeDeleteUser(msg);
			break;
		}

		TRACE("--------------------------------------")

	}
	catch (exception e)
	{
		TRACE("%s", e.what());
		safeDeleteUser(msg);
	}

}

void TriviaServer::safeDeleteUser(RecievedMessage* msg)
{
	try
	{
		if (msg->getUser()) //if there is a user
		{
			handleSignOut(msg);
			
		}
		TRACE("Closing socket = %d\n", msg->getSock());
		closesocket(msg->getSock());
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}
	
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

//sends feedback
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
	TRACE("Adding new user to connected users list: socket = %d, username = %s\n", msg->getSock(), username.c_str());
	
	Helper::sendData(msg->getSock(), "1020");
	TRACE("Message sent to user: %s, msg: %d", username.c_str(), msg->getSock());

	return newUser;
}

//sends feedback
bool TriviaServer::handleSignup(RecievedMessage* msg)
{
	string username = msg->getValues()[0];
	string password = msg->getValues()[1];
	string email = msg->getValues()[2];

	if (Validator::isPasswordValid(password) == false)
	{
		TRACE("SEND: User signup failed. Invalid password: username = %s, socket = %d", username.c_str(), msg->getSock())
		Helper::sendData(msg->getSock(), "1041");
		return false;
	}

	if (Validator::isUsernameValid(username) == false)
	{
		TRACE("SEND: User signup failed. Invalid username: username = %s, socket = %d", username.c_str(), msg->getSock())
		Helper::sendData(msg->getSock(), "1043");
		return false;
	}

	if (_db.isUserExists(username))
	{
		TRACE("SEND: User signup failed. User already exist: username = %s, socket = %d", username.c_str(), msg->getSock())
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

void TriviaServer::handleLeaveGame(RecievedMessage* msg)
{
	bool flag = msg->getUser()->leaveGame();
	if (flag)
	{
		msg->getUser()->getGame()->~Game();
	}
}

void TriviaServer::handleStartGame(RecievedMessage* msg)
{
	try
	{
		Game* g = new Game(msg->getUser()->getRoom()->getUsers(), msg->getUser()->getRoom()->getQuestionsNo(), _db);
		//ask Itay
	}
	catch (exception e)
	{
		msg->getUser()->send("1180");
	}
}

void TriviaServer::handlePlayerAnswer(RecievedMessage* msg)
{
	Game* g = msg->getUser()->getGame();
	if (g != NULL)
	{
		//g->handleAnswerFromUser(msg->getUser(), g->)
	}
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
//sends feedback
bool TriviaServer::handleJoinRoom(RecievedMessage* msg)
{
	User* user = msg->getUser();
	if (!user) //if no user
		return false;

	int roomId = atoi(msg->getValues()[0].c_str());
	Room* room = getRoomByld(roomId);
	if (!room) // no room
	{
		user->send("1102");
		return false;
	}

	//sends feedback(1101 and 1100)
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

	//sends to user if success
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
		user->send("1080");
		return;
	}

	string usersListMsg = room->getUsersListMessage();
	user->send(usersListMsg);
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
			
			if (roomName.size() < 10)
				sendMsg += '0';

			//add room's name size
			sendMsg += roomNameSize;


			sendMsg += roomName; //add room name

		}
	}

	user->send(sendMsg);
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

