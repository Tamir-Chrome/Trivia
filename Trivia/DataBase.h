#pragma once
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

class DataBase
{
public:
	DataBase();
	~DataBase();


	bool isUserExists(string);
	bool addNewUser(string, string, string);
	bool isUserAndPassMatch(string, string);
	//vector<Question*> initQuestions(int);
	//vector<string> getBestScores();
	//vector<string> getPersonalStatus(string);
	//int insertNewGame();
	//bool updateGameStatus(int);
	//bool addAnswerToPIayer(int, string, int, string, bool, int);
	
private:
	//TODO: add db callbacks functions
	fstream t_users;

};

