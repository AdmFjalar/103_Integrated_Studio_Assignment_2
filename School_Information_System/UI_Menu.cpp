#include "UI_Menu.h"
#include <iostream>;
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

using std::ofstream;
using std::ifstream;
using std::fstream;
using std::stringstream;
using std::ios;
using std::string;
using std::vector;
using std::cout;
using std::cin;

bool UI_Menu::TakeInput(string menuFile)
{
	fstream fileStream;

	fileStream.open(menuFile, ios::in | ios::beg);

	if (!fileStream.is_open())
	{
		cout << "Warning! File could not be opened.\n";
		return false;
	}

	string line, word;
	vector<string> row;
	vector<vector<string>> fileContent;

	while (getline(fileStream, line, '\n'))
	{
		row.clear();
		stringstream stream(line);
		while (getline(stream, word, ','))
		{
			row.push_back(word);
		}
		fileContent.push_back(row);
	}

	fileStream.close();

	switch (_getch()) {
	case UP_ARROW:
	{
		menuOptionIndex--;
		if (menuOptionIndex < 0)
		{
			menuOptionIndex = (fileContent.size() - 1);
		}
		return true;
		break;
	}
	case DOWN_ARROW:
	{
		menuOptionIndex++;
		if (menuOptionIndex >= fileContent.size())
		{
			menuOptionIndex = 0;
		}
		return true;
		break;
	}
	case RIGHT_ARROW:
	{
		UI_Menu* nextMenu = new UI_Menu();
		if (fileContent[menuOptionIndex].size() > 1)
		{
			while (nextMenu->PrintMenu(fileContent[menuOptionIndex][1]));
		}
		return true;
		break;
	}
	case LEFT_ARROW:
	{
		return false;
		break;
	}
	default:
	{
		break;
	}
	}
}

bool UI_Menu::PrintMenu(string menuFile)
{
	fstream fileStream;

	fileStream.open(menuFile, ios::in | ios::beg);

	if (!fileStream.is_open())
	{
		cout << "Warning! File could not be opened.\n";
		return false;
	}

	string line, word;
	vector<string> row;
	vector<vector<string>> fileContent;

	while (getline(fileStream, line, '\n'))
	{
		row.clear();
		stringstream stream(line);
		while (getline(stream, word, ','))
		{
			row.push_back(word);
		}
		fileContent.push_back(row);
	}

	fileStream.close();

	cout << "-----------------------------------------\n";
	cout << "--------- Navigate with arrow keys ------\n";
	cout << "------ < Exit menu --- Enter menu > -----\n";
	cout << "---------- ^ Go up --- Go down V --------\n";
	cout << "-----------------------------------------\n\n";

	cout << "-----------------------------------------\n";
	for (int i = 0; i < fileContent.size(); i++)
	{
		if (i == menuOptionIndex)
		{
			cout << "*";
		}
		else {
			cout << " ";
		}
		cout << " " << fileContent[i][0];
		cout << '\n';
	}
	cout << "-----------------------------------------\n";
	cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";

	return TakeInput(menuFile);
}