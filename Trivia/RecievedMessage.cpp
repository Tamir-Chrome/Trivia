#include "RecievedMessage.h"


RecievedMessage::RecievedMessage(SOCKET client, int code)
{
	_sock = client;
	_messageCode = code;

}


RecievedMessage::~RecievedMessage()
{
}
