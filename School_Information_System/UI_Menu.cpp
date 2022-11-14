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
bool UI_Menu::TakeArrowKeys(string menuFile, int id)
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
			if (fileContent[menuOptionIndex][1] != "" && fileContent[menuOptionIndex][2] != "")
			{
				while (nextMenu->PrintMenu(fileContent[menuOptionIndex][1], fileContent[menuOptionIndex][2], id));	//	Opens a new menu which will be looped until the user uses the left arrow to leave it.
				//	It loads the file location with the index of the currently selected option and selects
				//	the second cell of the corresponding row in the CSV file
			}
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

	vector<vector<string>> fileContent = ReadFile("userDataBase.csv");

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
			// checking if the name address is at 1 and the password address is at 2
			if ((name == row.at(1) && password == row.at(2)) || (name == row.at(0) && password == row.at(2))) {
				cout << "Welcome " << name << '\n';													//	Shows personal welcome message

				string userFileName = row.at(0);													
				userFileName.append(".csv");

				vector<vector<string>> userFileContent = ReadFile(userFileName);

				UI_Menu* uiMenu = new UI_Menu();

				// checking whether the user is a user or an admin
				switch (stoi(row.at(3)))
				{
				case 0:
				{
					//	User functionality
					cout << "Logged in as a student" << '\n';
					while (uiMenu->PrintMenu("studentStartMenu.csv", "arrow_keys", stoi(row.at(0))));
					return true;
					break;
				}

				case 1:
				{
					//	Guardian functionality
					cout << "Logged in as a guardian" << '\n';
					while (uiMenu->PrintMenu("guardianStartMenu.csv", "arrow_keys", stoi(row.at(0))));
					return true;
					break;
				}

				case 2:
				{
					//	Teacher functionality
					cout << "Logged in as a teacher" << '\n';
					while (uiMenu->PrintMenu("teacherStartMenu.csv", "arrow_keys", stoi(row.at(0))));
					return true;
					break;
				}

				case 3:
				{
					//	Admin functionality
					cout << "Logged in as an admin" << '\n';
					while (uiMenu->PrintMenu("adminStartMenu.csv", "arrow_keys", stoi(row.at(0))));
					return true;
					break;
				}
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

	fstream userDataBase("userDataBase.csv", ios::in);

	// conditional statement to check if the file is open
	if (!userDataBase.is_open()) {
		cout << "Warning file is not open" << '\n';
		return;
	}

	cout << "\n------------------------------- Create account -------------------------------" << "\n\n\n";

	// taking user input for username and password
	cout << "Enter your username: ";
	cin >> name;
	cout << "Enter your password: ";
	cin >> password;

	do {
		cout << "Enter account type : \n";
		cout << "0. Student\n";
		cout << "1. Guardian\n";
		cout << "2. Teacher\n";
		cin >> userAccessLevel;

		if (userAccessLevel < 0 || userAccessLevel > 2)
		{
			cout << "Invalid account type! Please try again.\n";
		}
	} while (userAccessLevel < 0 || userAccessLevel > 2);

	//------------------------------------------------------------------------------------------
	//--------------------------------- START OF ID ASSIGNMENT ---------------------------------
	//------------------------------------------------------------------------------------------

	srand(time(NULL));

	id = rand() % 500000 + 270000000;

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

	if (fileContent.size() > 0)
	{
		if (fileContent[0].size() > 0)
		{
			for (vector<string> row : fileContent)
			{
				while (row[0] == std::to_string(id))
				{
					id = rand() % 1000000 + 270000000;
				}
			}
		}
	}
	//------------- End of validating unique ID -------------

	//------------------------------------------------------------------------------------------
	//---------------------------------- END OF ID ASSIGNMENT ----------------------------------
	//------------------------------------------------------------------------------------------

	cout << "You've been assigned ID " << id << "\n\n";

	userDataBase.close();
	userDataBase.open("userDataBase.csv", ios::app);

	// storing input data into csv file
	userDataBase << id << "," << name << "," << password << "," << userAccessLevel << '\n';

	userDataBase.close();

	string userFile = std::to_string(id);
	userFile.append(".csv");

	userDataBase.open(userFile, ios::out);

	userDataBase << id << "\n" << name << "\n" << password << "\n" << userAccessLevel;

	userDataBase.close();
}

void UI_Menu::EditGrades(string courseName)
{
	string newGrade;
	int userInput;

	courseName.append(".csv");

	vector<vector<string>> fileContent = ReadFile(courseName);

	for (int i = 0; i < fileContent.size(); i++)
	{
		cout << i << ".\t";
		for (string cell : fileContent[i])
		{
			cout << cell << "\t";
		}
		cout << "\n";
	}

	do
	{
		cout << "Which student's grade would you like to change? (Enter index, or -1 to exit) : ";
		cin >> userInput;
		cout << "\n\n";

		if ((userInput < 0 || userInput >= fileContent.size()) && userInput != -1)
		{
			cout << "Warning! Invalid input. Try again.\n\n";
		}
	} while ((userInput < 0 || userInput >= fileContent.size()) && userInput != -1);

	if (userInput == -1)
	{
		cout << "Exiting menu.\n";
		return;
	}

	cout << "What grade would you like to assign " << fileContent[userInput][1] << "? : ";
	cin >> newGrade;
	cout << "\n\n";

	fileContent[userInput][2] = newGrade;

	fstream courseDataBase(courseName, ios::out);

	for (int i = 0; i < fileContent.size(); i++)
	{
		courseDataBase << fileContent[i][0] << "," << fileContent[i][1] << "," << fileContent[i][2];
		if (i != fileContent.size() - 1)
		{
			courseDataBase << "\n";
		}
	}

	// closing the .csv file
	courseDataBase.close();

	//clearing the file content vector
	fileContent.clear();
}

vector<vector<string>> UI_Menu::ReadFile(string fileName)
{
	fstream file;

	file.open(fileName, ios::in | ios::app);

	string line, cell;

	// Vectors to divide up .csv file into rows and cells
	vector<string> row;

	vector<vector<string>> fileContent;

	// conditional statement to check if the file is open
	if (!file.is_open()) {
		cout << "Warning file is not open" << '\n';
		return fileContent;
	}

	// reading the line in the userDataBase
	while (getline(file, line, '\n')) {
		row.clear();
		// assigning the variable line to stream
		stringstream stream(line);

		// reading through the variables
		while (getline(stream, cell, ',')) {
			row.push_back(cell);
		}
		fileContent.push_back(row);
	}

	file.close();

	return fileContent;
}

/// <summary>
/// Prints out menu options and adds an asterix ahead of the chosen option in-case the menu is indexable
/// </summary>
/// <param name="menuFile">File name of the current menu file</param>
/// <param name="inputType">Controls how the menu is printed and what options the user gets</param>
/// <returns>Returns if it should keep printing or not</returns>
bool UI_Menu::PrintMenu(string menuFile, string inputType, int id)
{
	string userFileName = std::to_string(id);
	userFileName.append(".csv");

	vector<vector<string>> userFileContent = vector<vector<string>>();
	
	if (id != 0)
	{
		userFileContent = ReadFile(userFileName);
	} 

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

			return TakeArrowKeys(menuFile, id);								//	Takes input for the menu, and returns if the user has selected to stay in the menu or not
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
	else if (inputType == "schoolInformation")
	{
		vector<vector<string>> schoolContent = ReadFile("school.csv");

		cout << "  " << schoolContent[0][0] << "\n\n";
		
		cout << "  Terms:\n";
		for (string term : schoolContent[2])
		{
			cout << "    " << term << '\n';
		}

		return TakeBackKey();
	}
	else if (inputType == "facultyList")
	{
		cout << "  Faculty :\n";

		vector<vector<string>> userDataBase = ReadFile("userDataBase.csv");

		for (vector<string> user : userDataBase)
		{
			string userFileName = user[0];
			userFileName.append(".csv");
			vector<vector<string>> userFile = ReadFile(userFileName);

			if (user[3] == std::to_string(2) || user[3] == std::to_string(3))
			{
				cout << "    " << user[1] << "\t";

				if (userFile.size() >= 5)
				{
					for (string cell : userFile[5])
					{
						cout << cell;
					}
				}
				cout << '\n';
			}
		}

		return TakeBackKey();
	}
	else if (inputType == "editGradeMenu")
	{
		for (int i = 0; i < userFileContent[4].size(); i++)
		{
			cout << i << ".\t" << userFileContent[4][i] << '\n';
		}
		cout << '\n';

		int courseSelection = 0;

		do
		{
			cin >> courseSelection;

			if (courseSelection < 0 || courseSelection >= userFileContent[4].size())
			{
				cout << "Invalid selection! Please try again.\n";
			}
		} while (courseSelection < 0 || courseSelection >= userFileContent[4].size());
		EditGrades(userFileContent[4][courseSelection]);
		return false;
	}
	else if (inputType == "guardianGrades")
	{

	}
	else if (inputType == "studentGrades")
	{
		for (int i = 0; i < userFileContent[4].size(); i++)
		{
			string tempString, displayString = userFileContent[4][i];

			tempString = displayString;
			tempString.append(".csv");

			displayString[i] = std::toupper(displayString[0]);

			vector<vector<string>> courseDataBase = ReadFile(tempString);
			for (vector<string> user : courseDataBase)
			{
				if (user[0] == std::to_string(id))
				{
					displayString.append(" :\t");
					displayString.append(user[2]);
				}
			}

			cout << "  " << displayString << '\n';
		}

		return TakeBackKey();
	}
	else if (inputType == "adminMenu")									//	Enters here if the user has chosen to load up an admin menu
	{
	}
	else {																//	Enters here if the menu csv file does not specify an input type
		cout << "ERROR! CSV menu file does not specify valid input type.\n";
		return false;
	}
}




// hide sidebar 