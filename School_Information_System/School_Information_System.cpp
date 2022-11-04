// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "UI_Menu.h";
#include "Base_Member.h";

using std::cout;
using std::cin;
using std::string;
using std::ios;
using std::vector;
using std::fstream;

void createUserAccount(string _name, string _password, int _userAccessLevel, int _id);
void userLogin(string _name, string _password);

int main()
{
	Base_Member* base_Member = new Base_Member();
	
	UI_Menu* uiMenu = new UI_Menu();

	string studentOptions[4] = {
		"Check grades",
		"Timetable",
		"Learning progress",
		"Classes"
	};

	while(uiMenu->PrintMenu(studentOptions, 4));

}


// -------------------------------------------- Login / Signup System --------------------------------------------

// function to handle users creating an account
void createUserAccount(string _name, string _password, int _userAccessLevel, int _id) {

	fstream userDataBase("userDataBase.csv", ios::in | ios::app);

	// conditional statement to check if the file is open
	if (!userDataBase.is_open()) {
		cout << "Warning file is not open" << '\n';
		return;
	}

	cout << "\n------------------------------- Create account -------------------------------" << "\n\n\n";

	// Need to add a id generator here:





	// taking user input for username and password
	cout << "Enter your username: ";
	cin >> _name;
	cout << "Enter your password: ";
	cin >> _password;

	// storing input data into csv file
	userDataBase << _id << _name << "," << _password << "," << _userAccessLevel << '\n';

	return;
}

// function to handle users logging in
void userLogin(string _name, string _password) {

	fstream userDataBase;

	userDataBase.open("userDataBase.csv", ios::in | ios::app);

	string line, word;

	// Vectors to divide up .csv file into rows and cells
	vector<string> row;

	vector<vector<string>> fileContent;

	// conditional statement to check if the file is open
	if (!userDataBase.is_open()) {
		cout << "Warning file is not open" << '\n';
		return;
	}

	cout << "\n------------------------------- User Login -------------------------------" << "\n\n\n";

	// taking user input for username and password
	cout << "Enter your username: ";
	cin >> _name;
	cout << "Enter your password: ";
	cin >> _password;


}