#pragma once
#include "User.h"
#include <vector>

class Room
{
public:
	Room(int id, User* admin, string name, int maxUsers, int questionsNo, int questionTime);
	~Room();

	bool joinRoom(User* user);
	void leaveRoom(User* user);
	int closeRoom(User* user);
	vector<User*> getUsers();
	string getUsersListMessage();
	int getQuestionsNo();
	int getId();
	string getName();

	
};

