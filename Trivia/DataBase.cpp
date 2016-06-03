#include "DataBase.h"


DataBase::DataBase()
{
	//db replacer
	//open file if not exist
	t_users.open("t_users.txt");
	//TODO: add exception if fails
	if (t_users.is_open())
		t_users.close();


}


DataBase::~DataBase()
{
	
}

bool DataBase::isUserExists(string username)
{
	t_users.open("t_users.txt");
	if (t_users.fail())
		return false;

	t_users.seekg(0); //seek to start

	while (!t_users.eof()) // reads each line from file
	{
		string name, pass, email;
		t_users >> name >> pass >> email;
		if (name == username)
		{
			//user found
			t_users.close();
			return true;
		}
	}
	t_users.close();
	//user found
	return false;
}

bool DataBase::addNewUser(string name, string pass, string email)
{
	t_users.open("t_users.txt");
	if (t_users.fail())
		return false;
	
	t_users.seekp(0, ios_base::end);
	t_users << name << " " << pass << " " << email << endl;
	t_users.close();
	return true;
}

bool DataBase::isUserAndPassMatch(string username, string password)
{
	t_users.open("t_users.txt");
	if (t_users.fail())
		return false;

	t_users.seekg(0); //seek to start

	while (!t_users.eof()) // reads each line from file
	{
		string name, pass, email;
		t_users >> name >> pass >> email;
		if (name == username && pass == password)
		{
			//match
			t_users.close();
			return true;
		}
	}
	t_users.close();
	//no match
	return false;
}