#include "Room.h"


Room::Room(int id, User* admin, string name, int maxUsers, int questionsNo, int questionTime)
{
	_id = id;
	_admin = admin;
	_name = name;
	_maxUsers = maxUsers;
	_questionsNo = questionsNo;
	_questionTime = questionTime;
}

Room::~Room()
{

}

bool Room::joinRoom(User* user)
{
	return false;
}

void Room::leaveRoom(User* user)
{

}

int Room::closeRoom(User* user)
{
	return 0;
}

vector<User*> Room::getUsers()
{
	return _users;
}

string Room::getUsersListMessage()
{
	return "";
}

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