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


	bool isUserExists(string username);
	bool addNewUser(string name, string pass, string email);
	bool isUserAndPassMatch(string username, string password);
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

