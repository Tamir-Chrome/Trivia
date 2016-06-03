#include "RecievedMessage.h"


RecievedMessage::RecievedMessage(SOCKET client, int code, vector<string> v)
{
	_sock = client;
	_messageCode = code;
	_values = v;
}


RecievedMessage::~RecievedMessage()
{
}
