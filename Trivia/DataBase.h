#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include "sqlite3.h"
#include "Question.h"
#include <vector>

using namespace std;

class DataBase
{
public:
	DataBase();
	~DataBase();


	bool isUserExists(string username);
	bool addNewUser(string name, string pass, string email);
	bool isUserAndPassMatch(string username, string password);
	vector<Question*> initQuestions(int questionsNo);
//	vector<string> getBestScores();
//	vector<string> getPersonalStatus(string);
	int insertNewGame();
	bool updateGameStatus(int game_id);
	bool addAnswerToPIayer(int gameId, string username, int questionId, string answer, bool isCorrect, int answerTime);
	
private:
	//TODO: add db callbacks functions
	sqlite3* _db;
	/*
	static int callbackCount(void* notUsed, int argc, char** argv, char** azCol);
	static int callbackQuestions(void* notUsed, int argc, char** argv, char** azCol);
	static int callbackBestScores(void* notUsed, int argc, char** argv, char** azCol);
	static int callbackPersonalStatus(void* notUsed, int argc, char** argv, char** azCol);
	*/
};

