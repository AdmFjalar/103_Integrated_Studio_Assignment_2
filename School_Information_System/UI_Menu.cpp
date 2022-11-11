#include "UI_Menu.h"		//	Includes headerfile for the class
#include <iostream>;		//	Input output stream library
#include <fstream>			//	Filestream library
#include <sstream>			//	Stringstream library
#include <string>			//	String library
#include <vector>			//	Vector library
#include <conio.h>			//	Console input output library
#include <time.h>			//	Time library

#define UP_ARROW 72			//	Defines the keycode for the up arrow, improving readability in the code
#define DOWN_ARROW 80		//	Defines the keycode for the down arrow, improving readability in the code
#define LEFT_ARROW 75		//	Defines the keycode for the left arrow, improving readability in the code
#define RIGHT_ARROW 77		//	Defines the keycode for the right arrow, improving readability in the code

using std::fstream;			//	Filestream
using std::stringstream;	//	Stringstream
using std::ios;				//	Data stream flags
using std::string;			//	String
using std::vector;			//	Vectors
using std::cout;			//	Output to console
using std::cin;				//	Input from console

/// <summary>
/// Lets user navigate menu with arrow keys, loads new menu by indexing current menu file when user presses right arrow
/// </summary>
/// <param name="menuFile">File name of the current menu file</param>
/// <returns>Returns if the user is to stay in the current menu</returns>
bool UI_Menu::TakeArrowKeys(string menuFile)
{
	fstream fileStream;												//	Declares the filestream used to load the contents of the menu csv file

	fileStream.open(menuFile, ios::in | ios::beg);					//	Opens specified menu file

	if (!fileStream.is_open())										//	Checks if the file failed to open
	{
		cout << "Warning! File could not be opened.\n";				//	Gives warning message
		return false;												//	Exits the function
	}

	string line, word;												//	Line is fed the rows, word is used for the cells of the table
	vector<string> row;												//	Stores individual cells in a row, allowing for indexing of the row
	vector<vector<string>> fileContent;								//	Stores the entire table of rows, allowing for indexing both x and y axis

	while (getline(fileStream, line, '\n'))							//	Goes through row for row and feeds each currently indexed row into line
	{
		row.clear();												//	Clears previously held row information
		stringstream stream(line);									//	Prepares the line for stream manipulation
		while (getline(stream, word, ','))							//	Goes through cell for cell and feeds each currently indexed cell into word
		{
			row.push_back(word);									//	Adds the currently indexed cell to the current row
		}
		fileContent.push_back(row);									//	Feeds the row into the table/matrix
	}

	fileStream.close();												//	Disassociates/closes the filestream

	switch (_getch()) {												//	Checks which key has been pressed
		case UP_ARROW:												//	Enters here when the up arrow has been pressed
		{
			menuOptionIndex--;										//	Iterates to the previous menu item (subtrating index since the menu is printed descending)
			if (menuOptionIndex < 0)								//	Checks if the user has scrolled outside of the allowed interval
			{
				menuOptionIndex = (fileContent.size() - 1);			//	Sets their choice to the opposite end of the menu
			}
			return true;											//	Returns that this menu should be kept open
			break;
		}
		case DOWN_ARROW:											//	Enters here when the down arrow has been pressed
		{
			menuOptionIndex++;										//	Iterates to the next menu item (adding index since the menu is printed descending)
			if (menuOptionIndex >= fileContent.size())				//	Checks if the user has scrolled outside of the allowed interval
			{
				menuOptionIndex = 0;								//	Sets their choice to the opposite end of the menu
			}
			return true;											//	Returns that this menu should be kept open
			break;
		}
		case RIGHT_ARROW:											//	Enters here when the right arrow has been pressed
		{
			UI_Menu* nextMenu = new UI_Menu();
			if (fileContent[menuOptionIndex].size() > 1)
			{
				while (nextMenu->PrintMenu(fileContent[menuOptionIndex][1], fileContent[menuOptionIndex][2]));	//	Opens a new menu which will be looped until the user uses the left arrow to leave it. 
																												//	It loads the file location with the index of the currently selected option and selects 
																												//	the second cell of the corresponding row in the CSV file
			}																			
			return true;											//	Returns that this menu should be kept open
			break;
		}
		case LEFT_ARROW:											//	Enters here when the left arrow has been pressed
		{
			return false;											//	Returns that this menu should not be kept open
			break;
		}
		default:													//	Enters here when any other key has been pressed
		{
			return true;											//	Returns that this menu should be kept open
			break;
		}
	}
}

/// <summary>
/// Only looks for left key, allowing to exit from a menu without indexing options
/// </summary>
/// <returns>Returns if the user has input to stay in the current menu</returns>
bool UI_Menu::TakeBackKey()
{
	if (_getch() == LEFT_ARROW)
	{
		return false;
	}
	else {
		return true;
	}
}

/// <summary>
/// Takes input for username and password, and validates against registered users 
/// </summary>
bool UI_Menu::UserLogin()
{
	string name, password;
	int numAttempts = 0;

	fstream userDataBase;

	userDataBase.open("userDataBase.csv", ios::in | ios::app);

	string line, word;

	// Vectors to divide up .csv file into rows and cells
	vector<string> row;

	vector<vector<string>> fileContent;

	// conditional statement to check if the file is open
	if (!userDataBase.is_open()) {
		cout << "Warning file is not open" << '\n';
		return false;
	}

	// reading the line in the userDataBase 
	while (getline(userDataBase, line, '\n')) {
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
	userDataBase.close();

	while (numAttempts < 3)
	{
		cout << "\n------------------------------- User Login -------------------------------" << "\n\n\n";

		// taking user input for username and password
		cout << "Enter your username: ";
		cin >> name;
		cout << "Enter your password: ";
		cin >> password;

		// looping through the vector 
		for (vector<string> row : fileContent) {
			// checking if the name address is at 0 and the password address is at 1
			if (name == row.at(0) && password.at(1)) {
				cout << "Welcome " << name << '\n';

				// checking whether the user is a user or an admin
				switch (stoi(row.at(2))) {

				case 0:
					// user functionality
					cout << "logged in as user" << '\n';
					return true;
					break;

				case 1:
					// admin functionality
					cout << "logged in as admin" << '\n';
					return true;
					break;
				}
			}
		}

		numAttempts++;
		if (numAttempts < 3)
		{
			cout << "Incorrect credentials! Try again. (" << 3 - numAttempts << " attempts left)\n";
		}
	}

	cout << "Too many attempts! Try again later.\n";

	return false;
}

/// <summary>
/// Create user account, take input for name and password, and assign ID
/// </summary>
void UI_Menu::CreateUserAccount()
{
	string name, password;
	int id, userAccessLevel;

	userAccessLevel = 0;
	id = 0;

	fstream userDataBase("userDataBase.csv", ios::in | ios::app);

	// conditional statement to check if the file is open
	if (!userDataBase.is_open()) {
		cout << "Warning file is not open" << '\n';
		return;
	}

	cout << "\n------------------------------- Create account -------------------------------" << "\n\n\n";

	//------------------------------------------------------------------------------------------
	//--------------------------------- START OF ID ASSIGNMENT ---------------------------------
	//------------------------------------------------------------------------------------------

	srand(time(NULL));

	id = rand() % 500000 + 270000000;

	// taking user input for username and password
	cout << "Enter your username: ";
	cin >> name;
	cout << "Enter your password: ";
	cin >> password;

	cout << "You've been assigned ID " << id << "\n\n";



	//---------------- Validating unique ID ----------------
	string line, word;												//	Line is fed the rows, word is used for the cells of the table
	vector<string> row;												//	Stores individual cells in a row, allowing for indexing of the row
	vector<vector<string>> fileContent;								//	Stores the entire table of rows, allowing for indexing both x and y axis

	while (getline(userDataBase, line, '\n'))	//	Goes through row for row and feeds each currently indexed row into line
	{
		row.clear();												//	Clears previously held row information
		stringstream stream(line);									//	Prepares the line for stream manipulation
		while (getline(stream, word, ','))		//	Goes through cell for cell and feeds each currently indexed cell into word
		{
			row.push_back(word);								//	Adds the currently indexed cell to the current row
		}
		fileContent.push_back(row);								//	Feeds the row into the table/matrix
	}

	for (vector<string> row : fileContent)
	{
		while (row[0] == std::to_string(id))
		{
			id = rand() % 1000000 + 270000000;
		}
	}
	//------------- End of validating unique ID -------------

	//------------------------------------------------------------------------------------------
	//---------------------------------- END OF ID ASSIGNMENT ----------------------------------
	//------------------------------------------------------------------------------------------


	// storing input data into csv file
	userDataBase << id << "," << name << "," << password << "," << userAccessLevel << '\n';
}

/// <summary>
/// Prints out menu options and adds an asterix ahead of the chosen option in-case the menu is indexable
/// </summary>
/// <param name="menuFile">File name of the current menu file</param>
/// <param name="inputType">Controls how the menu is printed and what options the user gets</param>
/// <returns>Returns if it should keep printing or not</returns>
bool UI_Menu::PrintMenu(string menuFile, string inputType)
{
	if (menuFile != "No_Menu_File")
	{
		fstream fileStream;												//	Declares the filestream used to load the contents of the menu csv file

		fileStream.open(menuFile, ios::in | ios::beg);		//	Opens specified menu file

		if (!fileStream.is_open())										//	Checks if the file failed to open
		{
			cout << "Warning! File could not be opened.\n";				//	Gives warning message
			return false;												//	Exits the function
		}

		string line, word;												//	Line is fed the rows, word is used for the cells of the table
		vector<string> row;												//	Stores individual cells in a row, allowing for indexing of the row
		vector<vector<string>> fileContent;								//	Stores the entire table of rows, allowing for indexing both x and y axis

		while (getline(fileStream, line, '\n'))		//	Goes through row for row and feeds each currently indexed row into line
		{
			row.clear();												//	Clears previously held row information
			stringstream stream(line);									//	Prepares the line for stream manipulation
			while (getline(stream, word, ','))		//	Goes through cell for cell and feeds each currently indexed cell into word
			{
				row.push_back(word);								//	Adds the currently indexed cell to the current row
			}
			fileContent.push_back(row);								//	Feeds the row into the table/matrix
		}

		fileStream.close();												//	Disassociates/closes the filestream

		if (inputType == "arrow_keys")
		{
			cout << "-----------------------------------------\n";		//	Outputs a line to outline instructions
			cout << "--------- Navigate with arrow keys ------\n";		//	Outputs instructions for how to navigate the menu
			cout << "------ < Exit menu --- Enter menu > -----\n";		//	Outputs instructions for how to navigate the menu
			cout << "---------- ^ Go up --- Go down V --------\n";		//	Outputs instructions for how to navigate the menu
			cout << "-----------------------------------------\n\n";	//	Outputs a line to outline instructions

			cout << "-----------------------------------------\n";		//	Outputs a line to outline menu
			for (int i = 0; i < fileContent.size(); i++)				//	Goes through all the options in the menu file
			{
				if (i == menuOptionIndex)								//	Checks if it's at the user's current selection
				{
					cout << "*";										//	Outputs an asteriks to show current selection
				}
				else {													//	Enters here if it's currently iterating anything other than the current user choice
					cout << " ";										//	Adds a space if it isn't the current selection so that all options will be at the same screen width
				}
				cout << " " << fileContent[i][0];						//	Outputs an extra space and the preview text for the menu option
				cout << '\n';											//	Adds a linebreak to separate menu options on different lines
			}
			cout << "-----------------------------------------\n";		//	Outputs a line to outline menu
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";				//	Adds 18 linebreaks so that only the current menu is visible

			return TakeArrowKeys(menuFile);								//	Takes input for the menu, and returns if the user has selected to stay in the menu or not
		}
		else if (inputType == "none")
		{
			cout << "-----------------------------------------\n";		//	Outputs a line to outline menu
			for (int i = 0; i < fileContent.size(); i++)				//	Goes through all the options in the menu file
			{
				cout << "  " << fileContent[i][0];						//	Outputs an extra space and the preview text for the menu option
				cout << '\n';											//	Adds a linebreak to separate menu options on different lines
			}
			cout << "-----------------------------------------\n";		//	Outputs a line to outline menu
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";				//	Adds 18 linebreaks so that only the current menu is visible
			return TakeBackKey();
		}
	}
	else if (inputType == "login")										//	Enters here and loads a login screen if the user has chosen a login option
	{
		UserLogin();
		return false;
	}
	else if (inputType == "signup") {									//	Enters here and loads a signup screen if the user has chosen a signup option
		CreateUserAccount();
		return false;
	}
	else if (inputType == "adminMenu")									//	Enters here if the user has chosen to load up an admin menu
	{

	}
	else {																//	Enters here if the menu csv file does not specify an input type
		cout << "ERROR! CSV menu file does not specify valid input type.\n";
		return false;
	}
}