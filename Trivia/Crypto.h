#pragma once

#include <string>
#include 

using namespace std;

class Crypto
{
public:
	static string encryptAES(string plainText);
	static string decryptAES(string cipherText);
};

