#include "DataBase.h"
#include <sstream>
#include "Helper.h"

vector<string> results;

void sqliteErr(char *zErrMsg)
{
	TRACE("SQL error: %s", zErrMsg);
	sqlite3_free(zErrMsg);
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

	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "select * from t_questions";

	rc = sqlite3_exec(_db, s.str().c_str(), getResults, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		sqliteErr(zErrMsg);
		return questions;
	}


	int randNum = rand() % (results.size() / 6); //right number is total number of questions
	int offset = randNum * 6; //index of a question id in results

	for (int i = 0; i < questionsNo; i++)
	{
		int id = atoi(results[offset].c_str());

		//check if already chose that question
		for each (Question* question in questions)
		{
			if (id == question->getId())
			{
				//skip iteration without losing a question
				i--;
				continue;
			}
		}

		Question* q = new Question(id, results[offset+1], results[offset+2], results[offset+3], results[offset+4], results[offset+5]);
		questions.push_back(q);
	}
	return questions;
}

/*waypoint 3
vector<string> DataBase::getBestScores()
{

}

vector<string> DataBase::getPersonalStatus(string)
{
	
}
*/ 
int DataBase::insertNewGame()
{
	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "INSERT INTO t_games(status, start_time, end_time) VALUES(0, NOW, NULL)";

	rc = sqlite3_exec(_db, s.str().c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		sqliteErr(zErrMsg);
		return -1;
	}

	int rowid = sqlite3_last_insert_rowid(_db);

	s << "select game_id from t_games where rowid == " << rowid;
	rc = sqlite3_exec(_db, s.str().c_str(), getResults, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		sqliteErr(zErrMsg);
		return -1;
	}

	int game_id = atoi(results[0].c_str());
	results.clear();

	return game_id;
}

bool DataBase::updateGameStatus(int game_id)
{
	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "update t_games set status=1, end_time=NOW where game_id == " << game_id;

	rc = sqlite3_exec(_db, s.str().c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		sqliteErr(zErrMsg);
		return false;
	}

	return true;
}

bool DataBase::addAnswerToPIayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime)
{
	int rc;
	char *zErrMsg = 0;
	stringstream s;

	s << "INSERT INTO t_players_answers(game_id, username, question_id, player_answer, is_correct, answer_time) VALUES(";
	s << gameId << ",'" << username << "'," << questionId << ",'" << answer << "'," << isCorrect << "," << answerTime << ")";

	rc = sqlite3_exec(_db, s.str().c_str(), NULL, 0, &zErrMsg);
	if (rc != SQLITE_OK)
	{
		sqliteErr(zErrMsg);
		return false;
	}

	return true;
}
