#include "Room.h"
#include <string>
#include "Helper.h"

Room::Room(int id, User* admin, string name, int maxUsers, int questionsNo, int questionTime)
{
	_id = id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
	_users.push_back(admin);
}

Room::~Room()
{
	
}

bool Room::joinRoom(User* user)
{
	//check if full
	if (_users.size() == _maxUsers)
	{
		user->send("1101");
		return false;
	}

	_users.push_back(user);

	string msg = "1100";

	if (_questionsNo < 10)
		msg += '0';
	msg += to_string(_questionsNo);

	if (_questionTime < 10)
		msg += '0';
	msg += to_string(_questionTime);

	user->send(msg);

	//send every user in the room the updated list
	sendMessage(getUsersListMessage());
}

void Room::leaveRoom(User* user)
{
	for (int i = 0; i < _users.size(); i++)
	{
		if (user == _users[i])
		{
			//remove the wanted user
			_users[i] = _users[_users.size() - 1];
			_users.pop_back();
			
			user->send("1120");

			sendMessage(user, getUsersListMessage());
		}
	}
}

int Room::closeRoom(User* user)
{
	//admin always in the first index
	//if user not admin 
	if (user != _users[0])
		return -1;

	sendMessage("116");

	//i = 1, skips admin
	for (int i = 1; i < _users.size(); i++)
		_users[i]->clearRoom();

	return _id;
}

vector<User*> Room::getUsers()
{
	return _users;
}

string Room::getUsersListMessage()
{
	string msg = "108";

	//add number of users
	msg += to_string(_users.size());

	for each (User* user in _users)
	{
		string username = user->getUsername();
		int usernameSize = username.size();

		//add username size
		if (usernameSize < 10)//if size is one digit
			msg += '0';

		msg += to_string(usernameSize);

		msg += username;
	}
	return msg;
};

int Room::getQuestionsNo()
{
	return _questionsNo;
}

int Room::getId()
{
	return _id;
}

string Room::getName()
{
	return _name;
}

string Room::getUsersAsString(vector<User*> usersList, User* excludeUser)
{
	string userList = "";
	for each (User* user in usersList)
	{
		if (user != excludeUser)
			userList += user->getUsername() + " ";
	}
	return userList;
}

void Room::sendMessage(string message)
{
	sendMessage(nullptr, message);
}

void Room::sendMessage(User* excludeUser, string message)
{
	try
	{
		for each (User* user in _users)
		{
			if (user != excludeUser)
				user->send(message);
		}
		
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}
}
