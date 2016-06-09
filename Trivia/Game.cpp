#include "Game.h"


Game::Game(const vector<User*>& players, int questionsNo, DataBase& db)
{
	
}

Game::~Game()
{

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