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

using std::fstream;
using std::stringstream;
using std::ios;
using std::string;
using std::vector;
using std::cout;
using std::cin;

bool UI_Menu::TakeInput(string menuFile)
{
	fstream fileStream;												//Declares the filestream used to load the contents of the menu csv file

	fileStream.open(menuFile, ios::in | ios::beg);		//Opens specified menu file

	if (!fileStream.is_open())										//Checks if the file failed to open
	{
		cout << "Warning! File could not be opened.\n";				//Gives warning message
		return false;												//Exits the function
	}

	string line, word;												//Line is fed the rows, word is used for the cells of the table
	vector<string> row;												//Stores individual cells in a row, allowing for indexing of the row
	vector<vector<string>> fileContent;								//Stores the entire table of rows, allowing for indexing both x and y axis

	while (getline(fileStream, line, '\n'))		//Goes through row for row and feeds each currently indexed row into line
	{
		row.clear();												//Clears previously held row information
		stringstream stream(line);									//Prepares the line for stream manipulation
		while (getline(stream, word, ','))		//Goes through cell for cell and feeds each currently indexed cell into word
		{
			row.push_back(word);								//Adds the currently indexed cell to the current row
		}
		fileContent.push_back(row);								//Feeds the row into the table/matrix
	}

	fileStream.close();												//Disassociates/closes the filestream

	switch (_getch()) {												//Checks which key has been pressed
	case UP_ARROW:													//Enters here when the up arrow has been pressed
	{
		menuOptionIndex--;											//Iterates to the previous menu item (subtrating index since the menu is printed descending)
		if (menuOptionIndex < 0)									//Checks if the user has scrolled outside of the allowed interval
		{
			menuOptionIndex = (fileContent.size() - 1);				//Sets their choice to the opposite end of the menu
		}
		return true;												//Returns that this menu should be kept open
		break;
	}
	case DOWN_ARROW:												//Enters here when the down arrow has been pressed
	{
		menuOptionIndex++;											//Iterates to the next menu item (adding index since the menu is printed descending)
		if (menuOptionIndex >= fileContent.size())					//Checks if the user has scrolled outside of the allowed interval
		{
			menuOptionIndex = 0;									//Sets their choice to the opposite end of the menu
		}
		return true;												//Returns that this menu should be kept open
		break;
	}
	case RIGHT_ARROW:												//Enters here when the right arrow has been pressed
	{
		UI_Menu* nextMenu = new UI_Menu();
		if (fileContent[menuOptionIndex].size() > 1)
		{
			while (nextMenu->PrintMenu(fileContent[menuOptionIndex][1]));	//Opens a new menu which will be looped until the user uses the left arrow to leave it. 
																					//It loads the file location with the index of the currently selected option and selects 
																					//the second cell of the corresponding row in the CSV file
		}																			
		return true;												//Returns that this menu should be kept open
		break;
	}
	case LEFT_ARROW:												//Enters here when the left arrow has been pressed
	{
		return false;												//Returns that this menu should not be kept open
		break;
	}
	default:														//Enters here when any other key has been pressed
	{
		return true;												//Returns that this menu should be kept open
		break;
	}
	}
}

bool UI_Menu::PrintMenu(string menuFile)
{
	fstream fileStream;												//Declares the filestream used to load the contents of the menu csv file

	fileStream.open(menuFile, ios::in | ios::beg);		//Opens specified menu file

	if (!fileStream.is_open())										//Checks if the file failed to open
	{
		cout << "Warning! File could not be opened.\n";				//Gives warning message
		return false;												//Exits the function
	}

	string line, word;												//Line is fed the rows, word is used for the cells of the table
	vector<string> row;												//Stores individual cells in a row, allowing for indexing of the row
	vector<vector<string>> fileContent;								//Stores the entire table of rows, allowing for indexing both x and y axis

	while (getline(fileStream, line, '\n'))		//Goes through row for row and feeds each currently indexed row into line
	{
		row.clear();												//Clears previously held row information
		stringstream stream(line);									//Prepares the line for stream manipulation
		while (getline(stream, word, ','))		//Goes through cell for cell and feeds each currently indexed cell into word
		{
			row.push_back(word);								//Adds the currently indexed cell to the current row
		}
		fileContent.push_back(row);								//Feeds the row into the table/matrix
	}

	fileStream.close();												//Disassociates/closes the filestream

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