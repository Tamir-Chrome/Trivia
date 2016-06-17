#pragma once
#include "socketAssist.h"
#include <vector>
#include "User.h"



using namespace std;

class RecievedMessage
{
public:
	RecievedMessage(SOCKET sock, int code);
	RecievedMessage(SOCKET sock, int code, vector<string> values);
	~RecievedMessage();

	SOCKET getSock();
	User* getUser();
	void setUser(User* user);
	int getMessageCode();
	vector<string>& getValues();

private:
	SOCKET _sock;
	User* _user;
	int _messageCode;
	vector<string> _values;
};

