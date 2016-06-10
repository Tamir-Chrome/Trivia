#pragma once
#include <string>

using namespace std;

class Question
{
public:
	Question(int id, string question, string correctAnswer, string answer2, string answer3, string answer4);
	
	string getQuestions();
	string* getAnswers();
	int getCorrectAnswerIndex();
	int getId();


private:
	string _question;
	string _answers[4];
	int _correctAnswerIndex;
	int _id;
};

