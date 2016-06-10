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

	/*
	rc = sqlite3_exec(_db, "CREATE TABLE t_users(username TEXT primary key not null, password TEXT not null, email TEXT not null)", NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
		sqliteErr(zErrMsg);
	*/

	/* 

		CREATE TABLE `t_users` (
		`username`	TEXT NOT NULL,
		`password`	TEXT NOT NULL,
		`email`	TEXT NOT NULL,
		PRIMARY KEY(username)
		);

		CREATE TABLE `t_games` (
		`game_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`status`	INTEGER NOT NULL,
		`start_time`	DATETIME NOT NULL,
		`end_time`	DATETIME
		);

		CREATE TABLE `t_questions` (
		`question_id`	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,
		`question`	TEXT NOT NULL,
		`correct_ans`	TEXT NOT NULL,
		`ans2`	TEXT NOT NULL,
		`ans3`	TEXT NOT NULL,
		`ans4`	TEXT NOT NULL
		);

		CREATE TABLE `t_players_answers` (
		`game_id`	INTEGER NOT NULL,
		`username`	TEXT NOT NULL,
		`question_id`	INTEGER NOT NULL,
		`player_answer`	TEXT NOT NULL,
		`is_correct`	INTEGER NOT NULL,
		`answer_time`	INTEGER NOT NULL,
		PRIMARY KEY(game_id,username,question_id)
		);
	*/

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

}

bool DataBase::updateGameStatus(int)
{

}

bool DataBase::addAnswerToPIayer(int, string, int, string, bool, int)
{

}
