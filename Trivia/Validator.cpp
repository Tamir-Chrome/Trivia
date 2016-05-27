//#include "Validator.h"

#include <string>

using namespace std;

bool isPasswordValid(string pass)
{
	if (pass.size() < 4)
		return false;

	if (pass.find(" ") != string::npos)
		return false;

	bool digitFlag = false, lowerFlag = false, upperFlag = false;

	for (int i = 0; i < pass.size(); i++)
	{
		if (isdigit(pass[i]))
			digitFlag = true;
		else if (islower(pass[i]))
			lowerFlag = true;
		else if (isupper(pass[i]))
			upperFlag = true;
	}
	if ((digitFlag && upperFlag && lowerFlag) == false)
		return false;

	return true;
}

bool isUsernameValid(string)
{
	return false;
}

