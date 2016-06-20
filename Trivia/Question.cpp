#include "Question.h"
#include <algorithm>

Question::Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4)
{
	_id = id;
	_question = question;

	_answers[0] = correctAnswer;
	_answers[1] = answer2;
	_answers[2] = answer3;
	_answers[3] = answer4;

	random_shuffle(std::begin(_answers), std::end(_answers));

	for (int i = 0; i < 4; i++)
	{
		if (_answers[i] == correctAnswer)
		{
			_correctAnswerIndex = i;
			break;
		}
	}
}

string Question::getQuestion()
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

