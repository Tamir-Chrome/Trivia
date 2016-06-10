#include "DataBase.h"
#include <sstream>
#include <vector>
#include "Helper.h"

vector<string> results;

void sqliteErr(char *zErrMsg)
{
	TRACE("SQL error: %s", zErrMsg);
	sqlite3_free(zErrMsg);
	system("Pause");
}


DataBase::DataBase()
{
		int rc;
	char *zErrMsg = 0;


	// connection to the database
	rc = sqlite3_open("trivia.db", &_db);

	if (rc)
	{
		cout << "Can't open database: " << sqlite3_errmsg(_db) << endl;
		sqlite3_close(_db);
		system("Pause");
		exit(1);
	}

}

DataBase::~DataBase()
{
	sqlite3_close(_db);
}

int getResults(void* notUsed, int argc, char** argv, char** azCol)
{
	for (int i = 0; i < argc; i++)
		results.push_back(argv[i]);
	
	return 0;

}

bool DataBase::isUserExists(string username)
{
	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "select * from t_users where username == '" << username << "'";

	rc = sqlite3_exec(_db, s.str().c_str(), getResults, 0, &zErrMsg);
	if (rc != SQLITE_OK)
		sqliteErr(zErrMsg);
	
	
	if (results.size() == 0) //no results
	{
		results.clear();
		return false;
	}

	//else
	results.clear();
	return true;
}

bool DataBase::addNewUser(string name, string pass, string email)
{
	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "insert into t_users (username, password, email) values( '" << name << "', '" << pass << "', '" << email << "')";

	rc = sqlite3_exec(_db, s.str().c_str(), NULL, 0, &zErrMsg);

	if (rc != SQLITE_OK) //error
	{
		sqliteErr(zErrMsg);
		return false;
	}

	//else
	return true;
}

bool DataBase::isUserAndPassMatch(string username, string password)
{
	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "select * from t_users where username == '" << username << "' and password == '" << password << "'";

	rc = sqlite3_exec(_db, s.str().c_str(), getResults, 0, &zErrMsg);
	if (rc != SQLITE_OK)
		sqliteErr(zErrMsg);
	

	if (results.size() != 0) //have results
	{
		results.clear();
		return true;
	}

	//else, no match
	results.clear();
	return false;
}

vector<Question*> DataBase::initQuestions(int questionsNo)
{
	vector<Question*> questions;
}

vector<string> DataBase::getBestScores()
{

}

vector<string> DataBase::getPersonalStatus(string)
{
	
}

int DataBase::insertNewGame()
{
	return 0;
}

bool DataBase::updateGameStatus(int)
{
	return false;
}

bool DataBase::addAnswerToPIayer(int, string, int, string, bool, int)
{
	return false;
}
