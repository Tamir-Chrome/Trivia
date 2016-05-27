#include "Validator.h"

//bool isPasswordValid(string pass)
//{
	
//}

bool Validator::isUsernameValid(string user)
{
	if ((int)user[0] < 65 || ((int)user[0]>90 && (int)user[0] < 97) || (int)user[0]>122)
	{
		return false;
	}
	if (user.find(" ") != -1)
	{
		return false;
	}
	if (user == "")
	{
		return false;
	}
	return true;
}

