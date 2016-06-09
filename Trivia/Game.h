#pragma once
#include "User.h"
#include <vector>
#include "DataBase.h"
class Game
{
public:
	Game(const vector<User*>&, int, DataBase&);
	~Game();
	void sendFirstQuestion();
	void handleFinishGame();
	bool handleNextTurn();
	bool handleAnswerFromUser(User*, int, int);
	bool leaveGame(User*);
	int getID();

private:
	bool insertGameToDB();
	void initQuestionsFromDB();
	void sendQuestionToAllUsers();
	
	//vector<Question*> _questions;
	vector<User*> _players;
	int _questions_no;
	int _currQuestionIndex;
};

