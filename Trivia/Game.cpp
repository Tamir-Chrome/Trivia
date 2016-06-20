#include "Game.h"
#include <string>

Game::Game(const vector<User*>& players, int questionsNo, DataBase& db) : _db(db)
{
	_questions_no = questionsNo;
	_currQuestionIndex = 0;
	_currentTurnAnswers = 0;
	try
	{
		_gameId = _db.insertNewGame();
		initQuestionsFromDB();
	}
	catch (exception e)
	{
		TRACE("%s", e.what());
	}
	
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
	_questions.clear();
	
	for (int i = _players.size()-1; i >= 0; i--)
	{
		_players[i]->clearRoom();
		_players.pop_back();
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
	
	mes = mes + "121";
	mes = mes + to_string(_players.size());
	for (int i = 0; i < _players.size(); i++)
	{
		string nameSize = to_string(_players[i]->getUsername().size());
		if (nameSize.size() < 10)
			mes = mes + '0';

		mes = mes + nameSize;

		mes = mes + _players[i]->getUsername();


		int points = _results[_players[i]->getUsername()];
		if (points < 10)
			mes = mes + '0';

		mes = mes + to_string(points);
	}
	for (int i = 0; i < _players.size(); i++)
	{
		try
		{
			_players[i]->send(mes);
		}
		catch (exception e)
		{
			TRACE("handleFinishGame: %s", e.what());
		}
	}
	
	
	for (int i = 0; i < _players.size(); i++)
	{
		_players[i]->setGame(nullptr);
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
	answerNo--;
	if (answerNo == _questions[_currQuestionIndex]->getCorrectAnswerIndex())
	{
		flag = true;
		success = _db.addAnswerToPIayer(_gameId, user->getUsername(), _questions[_currQuestionIndex]->getId(), _questions[_currQuestionIndex]->getAnswers()[answerNo], flag, time);
		_results[user->getUsername()]++;
	}
	else if (answerNo != 4)
		success = _db.addAnswerToPIayer(_gameId, user->getUsername(), _questions[_currQuestionIndex]->getId(), _questions[_currQuestionIndex]->getAnswers()[answerNo], flag, time);
	else
		success = _db.addAnswerToPIayer(_gameId, user->getUsername(), _questions[_currQuestionIndex]->getId(), "", flag, time);
	
	if (success == false)
		return false;

	string send = "120";

	send += (flag) ? "1" : "0";
	
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
	int id = _db.insertNewGame();
	if (id == -1)
		return false;

	_gameId = id;
	return true;
}

void Game::initQuestionsFromDB()
{
	_questions = _db.initQuestions(_questions_no);
}

void Game::sendQuestionToAllUsers()
{
	_currentTurnAnswers = 0;
	string msg = "118";

	//adds question
	string qSize = to_string(_questions[_currQuestionIndex]->getQuestion().size());

	for (int i = 0; i < 3 - qSize.size(); i++)//add padding zeros
		msg += '0';

	msg += qSize;
	msg += _questions[_currQuestionIndex]->getQuestion();

	//adds answrs
	string* answers = _questions[_currQuestionIndex]->getAnswers();

	for (int i = 0; i < 4; i++)
	{
		string aSize = to_string(answers[i].size());

		for (int i = 0; i < 3 - aSize.size(); i++)//add padding zeros
			msg += '0';

		msg += aSize;
		msg += answers[i];
	}

	for each (User* user in _players)
	{
		user->send(msg);
	}
}