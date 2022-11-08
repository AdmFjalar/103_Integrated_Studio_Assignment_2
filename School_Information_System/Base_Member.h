#pragma once
#include<string>

using std::string;

class Base_Member
{
	// stopping things from being accessed from anywhere within the program
private:
	string name, password;
	int age = 0, id = 0, userAccessLevel = 0;

	// allowing to be accessed from anywhere within the program
public:

	Base_Member();
	Base_Member(int _age, int _id, string _name, string _password, int _userAccessLevel);

	void createUserAccount(string _name, string _password, int _userAccessLevel, int _id);

	void userLogin(string _name, string _password);
};

