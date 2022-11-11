// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "UI_Menu.h";
#include "School.h";
#include "Base_Member.h";

using std::ofstream;
using std::ifstream;
using std::fstream;
using std::stringstream;
using std::ios;
using std::string;
using std::vector;
using std::cout;
using std::cin;

void teacherFunctionality();

int main()
{

	teacherFunctionality();

	UI_Menu* uiMenu = new UI_Menu();

	//UI_Menu studentMainMenuOptions[4] =
	//{
	//
	//}

	while (uiMenu->PrintMenu("mainmenu.csv", "arrow_keys"));
}

void teacherFunctionality() {

	string input;

	fstream englishDataBase;
	
	englishDataBase.open("english.csv", ios::in | ios::app);

	string line, word;

	// Vectors to divide up .csv file into rows and cells
	vector<string> row;

	vector<vector<string>> fileContent;

	// conditional statement to check if the file is open
	if (!englishDataBase.is_open()) {
		cout << "Warning file is not open" << '\n';
		return;
	}

	cout << "(testing purposes only)Enter data: ";
	cin >> input;

	englishDataBase << input << ",";

	// reading the line in the userDataBase 
	while (getline(englishDataBase, line, '\n')) {
		row.clear();
		// assigning the variable line to stream
		stringstream stream(line);

		// reading through the variables
		while (getline(stream, word, ',')) {
			row.push_back(word);
		}
		fileContent.push_back(row);
	}

	// closing the .csv file
	englishDataBase.close();

	//clearing the file content vector
	fileContent.clear();
}