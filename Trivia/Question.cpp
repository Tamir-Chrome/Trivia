#include "Question.h"
#include <time.h>

Question::Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4)
{
	srand(time(NULL));

	_answers[0] = correctAnswer;
	_answers[1] = answer2;
	_answers[2] = answer3;
	_answers[3] = answer4;

	_correctAnswerIndex = rand() % 4;

	//if the index is not the way is set to be (0)
	//swap the correct answer with a different answer
	if (_correctAnswerIndex != 0)
	{
		string temp = _answers[_correctAnswerIndex];
		_answers[_correctAnswerIndex] = correctAnswer;
		_answers[3 - _correctAnswerIndex] = temp;
	}
}

string Question::getQuestions()
{
	return _question;
}

string* Question::getAnswers()
{
	return _answers;
}

int Question::getCorrectAnswerIndex()
{
	return _correctAnswerIndex;
}

int Question::getId()
{
	return _id;
}

