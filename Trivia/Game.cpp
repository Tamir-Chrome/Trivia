#include "Game.h"


Game::Game(const vector<User*>& players, int questionsNo, DataBase& db) : _db(db)
{
	_questions_no = questionsNo;
	_players = players;
}

Game::~Game()
{
	/*
	for (int i = 0; i < _questions.size(); i++)
	{
		_questions[i]
	}
	*/
}

void Game::sendFirstQuestion()
{

}

void Game::handleFinishGame()
{

}

bool Game::handleNextTurn()
{
	return false;
}

bool Game::handleAnswerFromUser(User*, int, int)
{
	return false;
}

bool Game::leaveGame(User*)
{
	return false;
}

int Game::getID()
{
	return 0;
}

bool Game::insertGameToDB()
{
	return false;
}

void Game::initQuestionsFromDB()
{

}

void Game::sendQuestionToAllUsers()
{

}