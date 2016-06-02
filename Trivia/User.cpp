#include "User.h"


User::User(string name, SOCKET sock)
{
	_username = name;
	_sock = sock;
}


User::~User()
{
}

string User::getUsername()
{
	return _username;
}

SOCKET User::getScoket()
{
	return _sock;
}
