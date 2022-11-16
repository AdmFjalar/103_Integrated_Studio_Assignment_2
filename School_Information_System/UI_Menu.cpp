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

			delete(nextMenu);										//	Deletes nextMenu
			nextMenu = nullptr;										//	Sets nextMenu to a null pointer

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
	bool pressedLeftArrowKey = false;	//	Stores if the user has pressed the left key or not (only key allowed/necessary in this function)

	while (!pressedLeftArrowKey)		//	Loops while the user has not pressed the left key
	{
		if (_getch() == LEFT_ARROW)		//	Checks if the left key is pressed
		{
			pressedLeftArrowKey = true;	//	Stores that the user has pressed the left key (thus exiting the loop)
		}
	}

	return false;						//	Returns false (which represents that the user does NOT want to stay in the menu)
}

/// <summary>
/// Takes input for username and password, and validates against registered users
/// </summary>
bool UI_Menu::UserLogin()
{
	string name, password;												//	Used to store the user's input values for name and password
	int numAttempts = 0;												//	Stores how many login attempts the user has

	vector<vector<string>> fileContent = ReadFile("userDataBase.csv");	//	Reads file userDataBase.csv and stores contents in a type of matrix

	while (numAttempts < 3)												//	Loops while number of attempts is less than 3
	{
		cout << "\n------------------------------- User Login -------------------------------" << "\n\n\n";

		// taking user input for username and password
		cout << "Enter your username: ";
		cin >> name;																//	Stores user input for name
		cout << "Enter your password: ";
		cin >> password;															//	Stores user input for password

		
		for (vector<string> user : fileContent) {									//	Iterates through every user
			// Checking if the name or ID and the password corresponds to the current user
			if ((name == user.at(1) && password == user.at(2)) || (name == user.at(0) && password == user.at(2))) {
				cout << "Welcome " << name << '\n';									//	Shows personal welcome message

				string userFileName = user.at(0);									//	Stores file path to user file (user file name is the user ID of type CSV file)											
				userFileName.append(".csv");										//	Adds file type (CSV)

				vector<vector<string>> userFileContent = ReadFile(userFileName);	//	Reads user file and stores content in a matrix of sorts

				UI_Menu* uiMenu = new UI_Menu();									//	Creates a new instance of a UI_Menu									

				switch (stoi(user.at(3)))	// Checking whether the user is a student (0), guardian (1), teacher (2), or admin (3)
				{
					case 0:					//	Enters here if the user is a student
					{
						//	User functionality
						cout << "Logged in as a student" << '\n';
						while (uiMenu->PrintMenu("studentStartMenu.csv", "arrow_keys", stoi(user.at(0))));	//	Opens a menu from file studentStartMenu.csv with input type arrow_keys and ID of logged in user
						
						delete(uiMenu);		//	Deletes instance of uiMenu
						uiMenu = nullptr;	//	Sets uiMenu to a null pointer

						return true;		//	Returns to keep main menu open once the user has logged out (And also meaning the menu will reset, going back to the start)
						break;
					}

					case 1:					//	Enters here if the user is a guardian
					{
						//	Guardian functionality
						cout << "Logged in as a guardian" << '\n';
						while (uiMenu->PrintMenu("guardianStartMenu.csv", "arrow_keys", stoi(user.at(0))));	//	Opens a menu from file guardianStartMenu.csv with input type arrow_keys and ID of logged in user
						
						delete(uiMenu);		//	Deletes instance of uiMenu
						uiMenu = nullptr;	//	Sets uiMenu to a null pointer

						return true;		//	Returns to keep main menu open once the user has logged out (And also meaning the menu will reset, going back to the start)
						break;
					}

					case 2:					//	Enters here if the user is a teacher
					{
						//	Teacher functionality
						cout << "Logged in as a teacher" << '\n';
						while (uiMenu->PrintMenu("teacherStartMenu.csv", "arrow_keys", stoi(user.at(0))));	//	Opens a menu from file teacherStartMenu.csv with input type arrow_keys and ID of logged in user
						
						delete(uiMenu);		//	Deletes instance of uiMenu
						uiMenu = nullptr;	//	Sets uiMenu to a null pointer

						return true;		//	Returns to keep main menu open once the user has logged out (And also meaning the menu will reset, going back to the start)
						break;
					}

					case 3:					//	Enters here if the user is an admin
					{
						//	Admin functionality
						cout << "Logged in as an admin" << '\n';
						while (uiMenu->PrintMenu("adminStartMenu.csv", "arrow_keys", stoi(user.at(0))));	//	Opens a menu from file adminStartMenu.csv with input type arrow_keys and ID of logged in user
						
						delete(uiMenu);		//	Deletes instance of uiMenu
						uiMenu = nullptr;	//	Sets uiMenu to a null pointer

						return true;		//	Returns to keep main menu open once the user has logged out (And also meaning the menu will reset, going back to the start)
						break;
					}
				}

				delete(uiMenu);				//	Deletes instance of uiMenu
				uiMenu = nullptr;			//	Sets uiMenu to a null pointer
			}
		}

		numAttempts++;						//	Increases number of attempts
		if (numAttempts < 3)				//	Checks if the user has attempted less than 3 times
		{
			cout << "Incorrect credentials! Try again. (" << 3 - numAttempts << " attempts left)\n";
		}
	}

	cout << "Too many attempts! Try again later.\n";	//	Prints this if the user has attempted 3 or more times

	return false;										//	Returns false, closing the menu
}

/// <summary>
/// Create user account, take input for name and password, and assign ID
/// </summary>
void UI_Menu::CreateUserAccount()
{
	string name, password, email;															//	Stores users input values for name, password, and email
	int id, userAccessLevel;																//	Stores generated ID and chosen userAccessLevel (user type)

	userAccessLevel = 0;																	//	Defaults userAccessLevel (user type) to 0 (student)
	id = 0;																					//	Defaults ID to 0

	cout << "\n------------------------------- Create account -------------------------------" << "\n\n\n";

	// Taking user input for username, password, and email
	cout << "Enter your username: ";
	cin >> name;																			//	Stores input name
	cout << "Enter your password: ";
	cin >> password;																		//	Stores input password
	cout << "Enter your email: ";
	cin >> email;																			//	Stores input email

	do {
		cout << "Enter account type : \n";
		cout << "0. Student\n";
		cout << "1. Guardian\n";
		cout << "2. Teacher\n";
		cin >> userAccessLevel;																//	Stores input user type

		if (userAccessLevel < 0 || userAccessLevel > 2)										//	Checks if the user has chosen an invalid user type
		{
			cout << "Invalid account type! Please try again.\n";
		}
	} while (userAccessLevel < 0 || userAccessLevel > 2);									//	Restarts loop if the user has chosen an invalid user type

	//------------------------------------------------------------------------------------------
	//--------------------------------- START OF ID ASSIGNMENT ---------------------------------
	//------------------------------------------------------------------------------------------

	srand(time(NULL));																		//	Sets seed for randomization

	id = rand() % 500000 + 270000000;														//	Sets an ID between 270000000 - 270500000

	//---------------- Validating unique ID ----------------
	vector<vector<string>> fileContent = ReadFile("userDataBase.csv");						//	Reads userDataBase.csv and stores content in a matrix of sorts

	if (!fileContent.empty())																//	Checks if there's any content loaded from the file
	{
		if (!fileContent[0].empty())														//	Checks if first row has any content
		{
			for (vector<string> user : fileContent)											//	Loops through every user from the userdatabase
			{
				while (user[0] == std::to_string(id))										//	Loops while the generated ID is equal to the indexed user (to avoid duplicate ID's)
				{
					id = rand() % 1000000 + 270000000;										//	Generates a new ID
				}
			}
		}
	}
	//------------- End of validating unique ID -------------

	//------------------------------------------------------------------------------------------
	//---------------------------------- END OF ID ASSIGNMENT ----------------------------------
	//------------------------------------------------------------------------------------------

	cout << "You've been assigned ID " << id << "\n\n";

	fstream userDataBase("userDataBase.csv", ios::app);										//	Opens filestream to userDataBase.csv

	userDataBase << id << "," << name << "," << password << "," << userAccessLevel << '\n';	//	Stores user ID, name, password, and userAccessLevel (user type) in the userDataBase

	userDataBase.close();																	//	Closes / dereferences the filestream

	string userFile = std::to_string(id);													//	Creates a filepath equal to the user ID
	userFile.append(".csv");																//	Adds file type (.CSV) to file path

	userDataBase.open(userFile, ios::out);													//	Opens an output filestream to the user filepath

	userDataBase << id << "\n" << name << "\n" << password << "\n" << userAccessLevel << "\nplaceholder\n" << email << "\nplaceholder";	//	Stores ID, name, passowrd, userAccessLevel (user type), a placeholder (used for different things for different user types), email, and another placeholder

	userDataBase.close();																	//	Closes / dereferences the filestream

	cout << "Press left key to continue to main menu.\n";

	TakeBackKey();																			//	Waits for the user to press the back key before continuing;
}

void UI_Menu::EditGrades(string courseName)
{
	string newGrade;																		//	Used to store input new grade
	int userInput;																			//	Used to store input index choice for user

	courseName.append(".csv");																//	Adds file type (.CSV) to courseName

	vector<vector<string>> fileContent = ReadFile(courseName);								//	Reads course file and stores contents in a matrix of sorts

	for (int i = 0; i < fileContent.size(); i++)											//	Loops through the matrix rows
	{
		cout << i << ".\t";																	//	Outputs index as well as an indentation
		for (string cell : fileContent[i])													//	Loops through cells of the current row/user
		{
			cout << cell << "\t";															//	Outputs the value of the current cell (ID, name, or grade)
		}
		cout << "\n";																		//	Outputs a linebreak
	}

	do																						//	Loops atleast once
	{
		cout << "Which student's grade would you like to change? (Enter index, or -1 to exit) : ";
		cin >> userInput;																	//	Stores input index in userInput
		cout << "\n\n";																		//	Outputs 2 linebreaks

		if ((userInput < 0 || userInput >= fileContent.size()) && userInput != -1)			//	Checks if input is out of range/invalid
		{
			cout << "Warning! Invalid input. Try again.\n\n";
		}
	} while ((userInput < 0 || userInput >= fileContent.size()) && userInput != -1);		//	Loops while the input is out of range/invalid

	if (userInput == -1)																	//	Checks if the input is -1, meaning the user wants to leave the menu
	{
		cout << "Exiting menu.\n";
		return;																				//	Exits the function
	}

	cout << "What grade would you like to assign " << fileContent[userInput][1] << "? : ";	//	Outputs question/instruction as well as name of selected user
	cin >> newGrade;																		//	Stores input value in newGrade
	cout << "\n\n";																			//	Outputs 2 linebreaks

	fileContent[userInput][2] = newGrade;													//	Sets grade of selected student to newGrade

	fstream courseDataBase(courseName, ios::out);											//	Opens an output filestream to the selected coursefile

	for (int i = 0; i < fileContent.size(); i++)											//	Loops through the filecontent matrix
	{
		courseDataBase << fileContent[i][0] << "," << fileContent[i][1] << "," << fileContent[i][2] << '\n';	//	Inputs the current indexed row of the matrix into the coursefile

	}

	courseDataBase.close();																	//	Closes / dereferences the filestream

	fileContent.clear();																	//	Empties the filecontent matrix
}

vector<vector<string>> UI_Menu::ReadFile(string fileName)
{
	fstream file(fileName, ios::in);					//	Opens a filestream to fileName

	string line, cell;									//	Used to temporarily store line and cells of file

	vector<string> row;									//	Used to temporarily store row

	vector<vector<string>> fileContent;					//	Used to store file contents in a matrix

	// Checks if the file is open
	if (!file.is_open()) {
		cout << "Warning file is not open" << '\n';
		return fileContent;
	}

	while (getline(file, line, '\n')) {					//	Loops through every line, stores contents in line and indexes next line when it comes across a linebreak (\n)
		row.clear();									//	Clears previous row contents
		stringstream stream(line);						//	Opens a stringstream from line

		// reading through the variables
		while (getline(stream, cell, ',')) {			//	Loops through the stringstream, stores contents in cell and indexes next word when it comes across a comma (,)
			row.push_back(cell);						//	Adds the cell into the row
		}
		fileContent.push_back(row);						//	Adds the row to the filecontents matrix
	}

	file.close();										//	Closes / dereferences the filestream

	return fileContent;									//	Returns the filecontents
}

/// <summary>
/// Prints out menu options and adds an asterix ahead of the chosen option in-case the menu is indexable
/// </summary>
/// <param name="menuFile">File name of the current menu file</param>
/// <param name="inputType">Controls how the menu is printed and what options the user gets</param>
/// <returns>Returns if it should keep printing or not</returns>
bool UI_Menu::PrintMenu(string menuFile, string inputType, int id)
{
	string userFileName = std::to_string(id);							//	Stores filepath to user file
	userFileName.append(".csv");										//	Adds file type (.CSV) to user filepath

	vector<vector<string>> userFileContent = vector<vector<string>>();	//	Creates a matrix that will be used to store contents of the user file
	
	if (id != 0)														//	Checks if the ID isn't 0 (default)
	{
		userFileContent = ReadFile(userFileName);						//	Reads the user file and stores contents in the userFileContent matrix
	} 

	if (menuFile != "No_Menu_File")										//	Checks if the menuFile is not specified as no menu file
	{
		vector<vector<string>> fileContent = ReadFile(menuFile);		//	Reads the menuFile and stores the contents in the fileContent matrix

		if (inputType == "arrow_keys")									//	Checks if the inputType is set to arrow_keys
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

			return TakeArrowKeys(menuFile, id);							//	Takes input for the menu, and returns if the user has selected to stay in the menu or not
		}
		else if (inputType == "none")									//	Checks if the inputType is specified as none, thus meaning a display menu, not a navigational one
		{
			cout << "-----------------------------------------\n";		//	Outputs a line to outline menu
			for (int i = 0; i < fileContent.size(); i++)				//	Goes through all the options in the menu file
			{
				cout << "  " << fileContent[i][0];						//	Outputs an extra space and the preview text for the menu option
				cout << '\n';											//	Adds a linebreak to separate menu options on different lines
			}
			cout << "-----------------------------------------\n";		//	Outputs a line to outline menu
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";				//	Adds 18 linebreaks so that only the current menu is visible
			return TakeBackKey();										//	Waits until the user presses the back(left)arrow and then closes the menu
		}
	}							
	else if (inputType == "login")										//	Enters here and loads a login screen if the user has chosen a login option
	{
		UserLogin();													//	Calls login function
		return false;													//	Closes menu
	}
	else if (inputType == "signup") {									//	Enters here and loads a signup screen if the user has chosen a signup option
		CreateUserAccount();											//	Calls signup function
		return false;													//	Closes menu
	}
	else if (inputType == "schoolInformation")							//	Enters here and loads school information (name of school, term times)
	{
		vector<vector<string>> schoolContent = ReadFile("school.csv");	//	Reads school file contents and stores them in the schoolContent matrix

		cout << '\t' << schoolContent[0][0] << "\n\n";					//	Outputs an indentation and school name
		
		cout << "\tTerms:\n";
		for (string term : schoolContent[2])							//	Loops through all terms specified in the schoolContent
		{
			cout << "\t\t" << term << '\n';								//	Outputs 2 indentations and the currently indexed term
		}

		return TakeBackKey();											//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "facultyList")								//	Enters here and loads staff members and their contact details
	{
		cout << "  Faculty :\n";

		vector<vector<string>> userDataBase = ReadFile("userDataBase.csv");	//	Reads the user database and stores the contents in the userDataBase matrix

		for (vector<string> user : userDataBase)							//	Loops through every user in the user database
		{
			if (user[3] == "2" || user[3] == "3")							//	Checks if the indexed user is of userAccessLevel (user type) teacher (2) or admin (3)
			{
				string userFileName = user[0];								//	Copies user's ID for user filepath
				userFileName.append(".csv");								//	Adds filetype (.CSV) to user filepath
				vector<vector<string>> facultyMemberFileContent = ReadFile(userFileName);	//	Reads user file contents and saves them in the facultyMemberContent matrix
				cout << '\t' << user[1] << '\t';							//	Outputs indentation and user's name

				for (string contactMethod : facultyMemberFileContent[5])				//	Loops through the user's contact methods
				{
					cout << contactMethod;									//	Outputs contact method
				}
				cout << '\n';												//	Outputs a linebreak
			}
		}

		return TakeBackKey();												//	Waits until user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "editGradeMenu")								//	Enters here and lets user edit grades of any student enrolled in their courses
	{
		if (userFileContent[4][0] != "placeholder")						//	Checks if the first course specified in the user's file is not a placeholder
		{
			for (int i = 0; i < userFileContent[4].size(); i++)			//	Loops through the user's courses
			{
				cout << i << ".\t" << userFileContent[4][i] << '\n';	//	Outputs the index and name of the indexed course
			}
			cout << '\n';												//	Outputs a linebreak

			int courseSelection = 0;									//	Used to store user input for course selection

			do															//	Loops atleast once
			{
				cin >> courseSelection;									//	Takes input for course selection (by index, not name)

				if (courseSelection < 0 || courseSelection >= userFileContent[4].size())	//	Checks if the input value is out of bounds/invalid
				{
					cout << "Invalid selection! Please try again.\n";
				}
			} while (courseSelection < 0 || courseSelection >= userFileContent[4].size());	//	Loops while the input value is out of bounds/invalid
			EditGrades(userFileContent[4][courseSelection]);			//	Calls the editgrades function for the selected course
		}
		else {															//	Enters here if the user has no assigned courses (thus a default value of placeholder at the first course index)
			cout << "Warning! No courses assigned to this account.\n";
		}
		return TakeBackKey();													//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "guardianGrades")										//	Enters here and lets guardian see their children and any of their related courses as well as course progress
	{
		if (userFileContent[4][0] != "placeholder")								//	Checks if the first child specified in the user's file is not a placeholder
		{
			for (string child : userFileContent[4])								//	Loops through children in the userfile
			{
				string childFile = child;										//	Sets a filepath to the childfile with the child's ID
				childFile.append(".csv");										//	Adds filetype (.CSV) to the child's filepath
				vector<vector<string>> childFileContent = ReadFile(childFile);	//	Reads the child file and stores the contents in the childFileContent matrix

				cout << child << '\t' << childFileContent[1][0] << '\n';		//	Outputs the child's ID and name

				if (childFileContent[4][0] != "placeholder")					//	Checks if the child's first course isn't a placeholder
				{
					for (int i = 0; i < childFileContent[4].size(); i++)		//	Loops through the child's assigned courses
					{
						string tempString, displayString = childFileContent[4][i];	//	Tempstring used for filepath to course, displaystring is used to display the course name

						tempString = displayString;								//	Sets the tempstring equal to the displaystring
						tempString.append(".csv");								//	Sets the filetype (.CSV) to tempstring

						displayString[i] = std::toupper(displayString[0]);		//	Sets first letter of the displayString to a capital letter
						displayString.insert(0, ("\t"));						//	Inputs a tab in the beginning of the displayString

						vector<vector<string>> courseDataBase = ReadFile(tempString);	//	Loads course file (from tempString filepath) and stores contents in the courseDataBase matrix
						for (vector<string> user : courseDataBase)				//	Loops through every user in the course file
						{
							if (user[0] == child)								//	Checks if the indexed user is the indexed child
							{
								displayString.append(" :\t");					
								displayString.append(user[2]);					//	Appends grade of user to the displaystring
							}
						}

						cout << "\t" << displayString << '\n';					//	Outputs the displaystring
					}
				}
				else {															//	Enters here if the child has no courses assigned (thus having a default value of placeholder at index 0 of courses)
					cout << "\tNo courses assigned.\n";
				}
				cout << '\n';													//	Outputs a linebreak
			}
		}
		else {																	//	Enters here if the user has no children assigned
			cout << "Warning! No children assigned to your account.\n";
		}
		return TakeBackKey();													//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "studentGrades")								//	Enters here and lets student see their grades/progress in all their enrolled courses
	{
		if (userFileContent[4][0] != "placeholder")								//	Checks if the 
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
		}
		else {
			cout << "Warning! No courses have been assigned to your account.\n";
		}

		return TakeBackKey();
	}
	else if (inputType == "studentGuardians")							//	Enters here and lets student see all guardians/parents assigned to their account
	{
		if (userFileContent[6][0] != "placeholder")
		{
			for (string guardian : userFileContent[6])
			{
				string guardianFile = guardian;
				guardianFile.append(".csv");
				vector<vector<string>> guardianFileContent = ReadFile(guardianFile);

				cout << guardian << "  " << guardianFileContent[1][0] << '\n';
			}
		}
		else {
			cout << "Warning! No guardians assigned to your account.\n";
		}
		return TakeBackKey();
	}
	else if (inputType == "createCourse")								//Enters here and lets admin create a new course
	{
		bool uniqueCourseName = true;
		string nameOfCourse;

		do
		{
			uniqueCourseName = true;
			cout << "Please enter name of course : ";
			cin >> nameOfCourse;

			vector<vector<string>> coursesFile = ReadFile("courses.csv");

			if (coursesFile.size() > 0)
			{
				for (vector<string> course : coursesFile)
				{
					if (course[0] == nameOfCourse)
					{
						uniqueCourseName = false;
						cout << "Warning! Course with same name already exists. Try again.\n";
					}
				}
			}
		} while (!uniqueCourseName);

		fstream courses("courses.csv", ios::app);

		courses << nameOfCourse << "\n";

		courses.close();

		return false();
	}
	else if (inputType == "assignCourse")								//	Enters here and lets admin assign a user (teacher or student) a course
	{
		vector<vector<string>> userDataBaseContent = ReadFile("userDataBase.csv");

		string selectedUserID, selectedUserFile;
		int numOfStudents = 0, numOfTeachers = 0;

		for (vector<string> user : userDataBaseContent)
		{
			if (user[3] == "0")
			{
				cout << "STUDENT\t" << user[0] << '\t' << user[1] << '\n';
				numOfStudents++;
			}
			else if (user[3] == "2")
			{
				cout << "TEACHER\t" << user[0] << '\t' << user[1] << '\n';
			}
		}

		if (numOfStudents > 0 || numOfTeachers > 0)
		{
			bool selectedExistingUser = false;

			do
			{
				cout << "Please enter ID of user from list to assign a course.\n";
				cin >> selectedUserID;

				for (vector<string> user : userDataBaseContent)
				{
					if (user[0] == selectedUserID)
					{
						selectedExistingUser = true;

						if (user[3] == "0" || user[3] == "2")
						{
							selectedUserFile = selectedUserID;
							selectedUserFile.append(".csv");
							vector<vector<string>> selectedUserFileContent = ReadFile(selectedUserFile);

							vector<vector<string>> courses = ReadFile("courses.csv");

							if (courses.size() != 0)
							{
								bool selectedExistingCourse = false;

								string selectedCourse;

								do
								{
									for (vector<string> course : courses)
									{
										cout << "\t" << course[0] << "\n";
									}

									cout << "Please enter name of course you want to assign " << selectedUserFileContent[1][0] << "\n";
									cin >> selectedCourse;

									for (vector<string> course : courses)
									{
										if (course[0] == selectedCourse)
										{
											selectedExistingCourse = true;
										}
									}

									if (!selectedExistingCourse)
									{
										cout << "Warning! You selected an invalid course. Please check your spelling and try again.\n";
									}

									for (string userCourse : selectedUserFileContent[4])
									{
										if (userCourse == selectedCourse)
										{
											selectedExistingCourse = false;
											cout << "Warning! User already assigned to this course. Please select another.\n";
										}
									}
								} while (!selectedExistingCourse);

								if (selectedUserFileContent[4][0] == "placeholder")
								{
									selectedUserFileContent[4][0] = selectedCourse;
								}
								else {
									selectedUserFileContent[4].push_back(selectedCourse);
								}

								fstream changeUserFile(selectedUserFile, ios::out);

								for (int i = 0; i < selectedUserFileContent.size(); i++)
								{
									for (int j = 0; j < selectedUserFileContent[i].size(); j++)
									{
										changeUserFile << selectedUserFileContent[i][j];
										if (j != selectedUserFileContent[i].size() - 1)
										{
											changeUserFile << ",";
										}
									}

									if (i != selectedUserFileContent.size() - 1)
									{
										changeUserFile << "\n";
									}
								}

								changeUserFile.close();

								if (user[3] == "0")
								{
									selectedCourse.append(".csv");

									fstream selectedCourseFile(selectedCourse, ios::app);

									selectedCourseFile << selectedUserFileContent[0][0] << "," << selectedUserFileContent[1][0] << "," << "D" << "\n";

									selectedCourseFile.close();
								}

								cout << "Added user to course.\n";

								return false;
							}
							else {
								cout << "Warning! No existing courses.\n";
								return TakeBackKey();
							}
						}
						else {
							cout << "Warning! You selected an invalid account-type. Try again.\n";
							return TakeBackKey();
						}
					}
				}
			} while (!selectedExistingUser);
		}
		else {
			cout << "Warning! No student or teacher accounts found on record.\n";
		}


		return TakeBackKey();
	}
	else if (inputType == "assignGuardian")								//	Enters here and lets admin assign a student a guardian
	{
		vector<vector<string>> userDataBaseContent = ReadFile("userDataBase.csv");

		string selectedStudentID, selectedGuardianID;

		int numOfStudents = 0, numOfGuardians = 0;

		for (vector<string> user : userDataBaseContent)
		{
			if (user[3] == "0")
			{
				numOfStudents++;
				cout << "  " << user[0] << "\t" << user[1] << '\n';
			}
			else if (user[3] == "1")
			{
				numOfGuardians++;
			}
		}

		if (numOfStudents > 0 && numOfGuardians > 0)
		{
			cout << "Please enter ID of student from list to assign a guardian.\n";
			cin >> selectedStudentID;

			for (vector<string> user : userDataBaseContent)
			{
				if (user[0] == selectedStudentID)
				{
					if (user[3] == "0")
					{
						string selectedGuardianFile, selectedStudentFile = selectedStudentID;
						selectedStudentFile.append(".csv");

						vector<vector<string>> selectedStudentFileContent = ReadFile(selectedStudentFile);

						if (selectedStudentFileContent.size() > 0)
						{
							for (vector<string> _user : userDataBaseContent)
							{
								if (_user[3] == "1")
								{
									cout << "  " << _user[0] << "\t" << _user[1] << '\n';
								}
							}

							cout << "Please enter ID of guardian from list to assign a guardian.\n";
							cin >> selectedGuardianID;

							for (vector<string> _user : userDataBaseContent)
							{
								if (_user[0] == selectedGuardianID)
								{
									if (_user[3] == "1")
									{
										selectedGuardianFile = selectedGuardianID;
										selectedGuardianFile.append(".csv");

										vector<vector<string>> selectedGuardianFileContent = ReadFile(selectedGuardianFile);

										if (selectedGuardianFileContent[4][0] == "placeholder")
										{
											selectedGuardianFileContent[4][0] = selectedStudentID;
										}
										else {
											selectedGuardianFileContent[4].push_back(selectedStudentID);
										}

										if (selectedStudentFileContent[6][0] == "placeholder")
										{
											selectedStudentFileContent[6][0] = selectedGuardianID;
										}
										else {
											selectedStudentFileContent[6].push_back(selectedGuardianID);
										}

										fstream changeUserFile(selectedStudentFile, ios::out);

										for (int i = 0; i < selectedStudentFileContent.size(); i++)
										{
											for (int j = 0; j < selectedStudentFileContent[i].size(); j++)
											{
												changeUserFile << selectedStudentFileContent[i][j];
												if (j != selectedStudentFileContent[i].size() - 1)
												{
													changeUserFile << ",";
												}
											}

											if (i != selectedStudentFileContent.size() - 1)
											{
												changeUserFile << "\n";
											}
										}

										changeUserFile.close();

										changeUserFile.open(selectedGuardianFile, ios::out);

										for (int i = 0; i < selectedGuardianFileContent.size(); i++)
										{
											for (int j = 0; j < selectedGuardianFileContent[i].size(); j++)
											{
												changeUserFile << selectedGuardianFileContent[i][j];
												if (j != selectedGuardianFileContent[i].size() - 1)
												{
													changeUserFile << ",";
												}
											}

											if (i != selectedGuardianFileContent.size() - 1)
											{
												changeUserFile << "\n";
											}
										}

										changeUserFile.close();

										cout << "Guardian assigned.\n";
										return false;
									}
									else
									{
										cout << "Selected user is not a guardian account.\n";
									}
								}
							}
						}
					}
					else {
						cout << "Selected user is not a student account.\n";
					}
				}
			}
		}
		else if (numOfStudents <= 0)
		{
			cout << "Warning! No student accounts found on record.\n";
		}
		else if (numOfGuardians <= 0)
		{
			cout << "Warning! No guardian accounts found on record.\n";
		}

		return TakeBackKey();
	}
	else if (inputType == "studentSupportReport")						//	Enters here and lets admin see which students are struggling as well as any guardians (and the guardians contact details)
	{
		vector<vector<string>> courses = ReadFile("courses.csv");

		for (vector<string> course : courses)
		{
			string courseNameUpper = course[0];
			courseNameUpper[0] = std::toupper(courseNameUpper[0]);
			cout << courseNameUpper << '\n';

			string courseFile = course[0];
			courseFile.append(".csv");

			vector<vector<string>> courseFileContent = ReadFile(courseFile);

			for (vector<string> student : courseFileContent)
			{
				if (student[2] == "F" || student[2] == "D")
				{
					string studentFile = student[0];
					studentFile.append(".csv");
					vector<vector<string>> studentFileContent = ReadFile(studentFile);

					cout << student[0] << '\t' << student[1] << '\t' << student[2] << '\t' << " : May require extra support.\n";
					if (studentFileContent[6][0] != "placeholder")
					{
						for (string guardian : studentFileContent[6])
						{
							string guardianFile = guardian;
							guardianFile.append(".csv");
							vector<vector<string>> guardianFileContent = ReadFile(guardianFile);

							cout << guardianFileContent[1][0] << '\t';

							if (guardianFileContent[5][0] != "placeholder")
							{
								for (string contactMethod : guardianFileContent[5])
								{
									cout << contactMethod << '\t';
								}
							}
							else {
								cout << "No contact method found";
							}
							cout << '\n';
						}
					}
					else {
						cout << "No guardian assigned" << '\t';
					}
					cout << '\n';
				}
			}
		}

		return TakeBackKey();
	}
	else {																//	Enters here if the menu csv file does not specify an input type
		cout << "ERROR! CSV menu file does not specify valid input type.\n";
		return false;
	}
}