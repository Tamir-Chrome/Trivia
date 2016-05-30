#pragma once
#include "socketAssist.h"
#include <vector>
#include "User.h"


using namespace std;

class RecievedMessage
{
public:
	RecievedMessage(SOCKET, int);
	RecievedMessage(SOCKET, int, vector<string>);
	~RecievedMessage();

	SOCKET getSock();
	User* getUser();
	void setUser();
	int getMessageCode();
	vector<string>& getValues();

private:
	SOCKET _sock;
	User* _user;
	int _messageCode;
	vector<string> _values;
};

