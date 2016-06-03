#include "RecievedMessage.h"


RecievedMessage::RecievedMessage(SOCKET sock, int code) : _sock(sock), _messageCode(code)
{

}


RecievedMessage::RecievedMessage(SOCKET client, int code, vector<string> values) : _sock(client), _messageCode(code), _values(values)
{

}

RecievedMessage::~RecievedMessage()
{

}


SOCKET RecievedMessage::getSock()
{
	return _sock;
}

User* RecievedMessage::getUser()
{
	return _user;
}

void RecievedMessage::setUser(User* user)
{
	_user = user;
}

int RecievedMessage::getMessageCode()
{
	return _messageCode;
}

vector<string>& RecievedMessage::getValues()
{
	return _values;
}


