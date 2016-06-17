#include "Game.h"


Game::Game(const vector<User*>& players, int questionsNo, DataBase& db) : _db(db)
{
	_questions_no = questionsNo;
	_currQuestionIndex = 0;
	_currentTurnAnswers = 0;
	try
	{
		_gameId = _db.insertNewGame();
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}
	_questions = _db.initQuestions(_questions_no);
	_players = players;
	for (int i = 0; i < _players.size(); i++)
	{
		_results.insert(std::pair<string, int>(_players[i]->getUsername(), 0));
	}
	for (int i = 0; i < _players.size(); i++)
	{
		_players[i]->setGame(this);
	}
}

Game::~Game()
{
	for (int i = 0; i < _questions.size(); i++)
	{
		delete _questions[i];
	}
	for (int i = 0; i < _players.size(); i++)
	{
		delete _players[i];
	}
}

void Game::sendFirstQuestion()
{
	sendQuestionToAllUsers();
}

void Game::handleFinishGame()
{
	string mes = "";
	_db.updateGameStatus(_gameId);
	try
	{
		mes = mes + "121";
		mes = mes + to_string(_players.size());
		for (int i = 0; i < _players.size(); i++)
		{
			mes = mes + to_string(_players[i]->getUsername().size());
			mes = mes + _players[i]->getUsername();
			mes = mes + to_string(_results[_players[i]->getUsername()]);
		}
		for (int i = 0; i < _players.size(); i++)
		{
			Helper::sendData(_players[i]->getScoket(), mes);
		}
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}
	for (int i = 0; i < _players.size(); i++)
	{
		delete _players[i];
	}
}

bool Game::handleNextTurn()
{
	if (_players.size() > 0)
	{
		if (_currentTurnAnswers == _players.size())
		{
			if (_currQuestionIndex + 1 == _questions_no)
			{
				handleFinishGame();
				return false;
			}
			else
			{
				_currQuestionIndex++;
				sendQuestionToAllUsers();
				return true;
			}
		}
		return true;
	}
	else
	{
		handleFinishGame();
		return false;
	}
}

bool Game::handleAnswerFromUser(User* user, int answerNo, int time)
{
	_currentTurnAnswers++;
	bool flag = false;
	bool success;
	if (answerNo == _questions[_currQuestionIndex]->getCorrectAnswerIndex())
	{
		_results[user->getUsername()]++;
		flag = true;
	}
	if (answerNo != 5)
	{
		success = _db.addAnswerToPIayer(_gameId, user->getUsername(), _questions[_currQuestionIndex]->getId(), _questions[_currQuestionIndex]->getAnswers()[answerNo], flag, time);
	}
	else
	{
		success = _db.addAnswerToPIayer(_gameId, user->getUsername(), _questions[_currQuestionIndex]->getId(), "", flag, time);
	}
	string send = "";
	send = send + "120";
	if (flag)
	{
		send = send + "1";
	}
	else
	{
		send = send + "0";
	}
	Helper::sendData(user->getScoket(), send);
	return handleNextTurn();
}

bool Game::leaveGame(User* currUser)
{
	bool flag = false;
	int i = -1;
	while (!flag)
	{
		i++;
		if (_players[i] == currUser)
		{
			flag = true;
			_players.erase(_players.begin() + i);
		}
	}
	return handleNextTurn();
}

int Game::getID()
{
	return _gameId;
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