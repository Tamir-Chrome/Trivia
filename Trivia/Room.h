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

private:
	vector<User*> _users;
	User* _admin;
	int _maxUsers;
	int _questionTime;
	int _questionsNo;
	string _name;
	int _id;


	string getUsersAsString(vector<User*> usersList, User* excludeUser);
	void sendMessage(string message);
	void sendMessage(User* excludeUser, string message);
};

