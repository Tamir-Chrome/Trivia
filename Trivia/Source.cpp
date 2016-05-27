#include <iostream>
#include <string>
using namespace std;
void main()
{
	string user = "1ser";
	if ((int)user[0] < 65 || ((int)user[0]>90 && (int)user[0] < 97) || (int)user[0]>122)
	{
		cout << "false" << endl;
	}
	if (user.find(" ") != -1)
	{
		cout << "false" << endl;
	}
	if (user == "")
	{
		cout << "false" << endl;
	}
	cout << "true" << endl;
	system ("PAUSE");
}