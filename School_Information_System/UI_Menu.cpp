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

bool UI_Menu::TakeArrowKeys(string menuFile, int id)
{
	fstream fileStream;												//	Declares the filestream used to load the contents of the menu csv file

	fileStream.open(menuFile, ios::in | ios::beg);					//	Opens specified menu file

	if (!fileStream.is_open())										//	Checks if the file failed to open
	{
		cout << "Warning! File could not be opened.\n";				//	Gives warning message
		return TakeBackKey();										//	Waits until the user presses the back(left) arrow and then closes the menu
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

	if (!userDataBase.is_open())															//	Checks if the filestream failed to open
	{
		cout << "Warning! File couldn't be opened.\n";
		
		TakeBackKey();																		//	Waits until the user presses the back(left) arrow and then closes the menu
		return;
	}

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

	if (!courseDataBase.is_open())												//	Checks if the filestream failed to open
	{
		cout << "Warning! File couldn't be opened.\n";
		TakeBackKey();											//	Waits until the user presses the back(left) arrow and then closes the menu
		return;
	}

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

void UI_Menu::EditFile(vector<vector<string>> fileContent, string filePath)
{
	fstream editFile(filePath, ios::out);					//	Opens an output filestream

	for (int i = 0; i < fileContent.size(); i++)			//	Loops through the fileContent rows
	{
		for (int j = 0; j < fileContent[i].size(); j++)		//	Loops through the row's cells
		{
			editFile << fileContent[i][j];					//	Writes the indexed cell to the editFile

			if (j != fileContent[i].size() - 1)				//	Checks if the current cell is not the last in the row
			{
				editFile << ",";							//	Writes a comma (,) to the editFile
			}
		}

		if (i != fileContent.size() - 1)					//	Checks if the row is not the last in the fileContent matrix
		{
			editFile << "\n";								//	Writes a linebreak (\n) to the editFile
		}
	}

	editFile.close();										//	Closes / dereferences the filestream
}

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
	else if (inputType == "facultyList")									//	Enters here and loads staff members and their contact details
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
	else if (inputType == "studentGrades")										//	Enters here and lets student see their grades/progress in all their enrolled courses
	{
		if (userFileContent[4][0] != "placeholder")								//	Checks if the students first course isn't a placeholder
		{
			for (int i = 0; i < userFileContent[4].size(); i++)					//	Loops through the user's courses
			{
				string tempString, displayString = userFileContent[4][i];		//	Tempstring used for filepath to course, displaystring is used to display the course name	

				tempString = displayString;										//	Sets the tempstring equal to the displaystring
				tempString.append(".csv");										//	Sets the filetype (.CSV) to tempstring

				displayString[i] = std::toupper(displayString[0]);				//	Sets first letter of the displayString to a capital letter
				displayString.insert(0, ("\t"));								//	Inputs a tab in the beginning of the displayString

				vector<vector<string>> courseDataBase = ReadFile(tempString);	//	Loads course file (from tempString filepath) and stores contents in the courseDataBase matrix
				for (vector<string> user : courseDataBase)						//	Loops through every user in the course file
				{
					if (user[0] == std::to_string(id))							//	Checks if the indexed user is the user
					{
						displayString.append(" :\t");
						displayString.append(user[2]);							//	Appends grade of user to the displaystring
					}
				}

				cout << "\t" << displayString << '\n';							//	Outputs the displaystring
			}
		}
		else {																	//	Enters here if the user's first course is a placeholder, thus having no assigned courses
			cout << "Warning! No courses have been assigned to your account.\n";
		}

		return TakeBackKey();													//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "studentGuardians")											//	Enters here and lets student see all guardians/parents assigned to their account
	{
		if (userFileContent[6][0] != "placeholder")										//	Checks if the user's first guardian isn't a placeholder
		{
			for (string guardian : userFileContent[6])									//	Loops through all the guardians assigned to the user
			{
				string guardianFile = guardian;											//	Sets a filepath to the guardians userfile by their ID
				guardianFile.append(".csv");											//	Adds filetype (.CSV) to the guardian filepath
				vector<vector<string>> guardianFileContent = ReadFile(guardianFile);	//	Reads the guardians userfile and stores the contents in the guardianFileContent matrix

				cout << guardian << '\t' << guardianFileContent[1][0] << '\n';			//	Outputs the guardians id and name
			}
		}
		else {																			//	Enters here if the first guardian is a placeholder, thus having no guardians assigned
			cout << "Warning! No guardians assigned to your account.\n";
		}
		return TakeBackKey();															//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "createCourse")									//	Enters here and lets admin create a new course
	{
		bool uniqueCourseName = true;										//	Stores whether the user has entered a unique coursename (defaults to true)
		string nameOfCourse;												//	Stores desired name of course

		do																	//	Loops atleast once
		{
			uniqueCourseName = true;										//	Resets uniqueCourseName to true
			cout << "Please enter name of course : ";					
			cin >> nameOfCourse;											//	Stores desired coursename

			vector<vector<string>> coursesFileContent = ReadFile("courses.csv");	//	Reads the courses file and stores the content in the coursesFileContent matrix

			if (!coursesFileContent.empty())								//	Checks whether the courses file is not empty
			{
				for (vector<string> course : coursesFileContent)			//	Loops through every course in the courses file
				{
					if (course[0] == nameOfCourse)							//	Checks if the desired coursename is equal to the indexed course
					{
						uniqueCourseName = false;							//	Stores that the name is not unique
						cout << "Warning! Course with same name already exists. Try again.\n";
					}
				}
			}
		} while (!uniqueCourseName);										//	Loops while the course name is not unique

		fstream courses("courses.csv", ios::app);							//	Opens an appending filestream to the courses file

		if (!courses.is_open())												//	Checks if the filestream failed to open
		{
			cout << "Warning! File couldn't be opened.\n";
			return TakeBackKey();											//	Waits until the user presses the back(left) arrow and then closes the menu
		}

		courses << nameOfCourse << "\n";									//	Outputs the nameOfCourse (desired course name) to the courses file

		courses.close();													//	Closes / dereferences the filestream

		cout << "Successfully added course!\n";

		return TakeBackKey();												//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "assignCourse")																	//	Enters here and lets admin assign a user (teacher or student) a course
	{
		vector<vector<string>> userDataBaseContent = ReadFile("userDataBase.csv");							//	Reads the userdatabase and stores the content in the userDataBaseContent matrix

		string selectedUserID, selectedUserFile;															//	selectedUserID is used to select a user to assign the course, selectedUserFile is the corresponding filepath to that user
		int numOfStudents = 0, numOfTeachers = 0;															//	Stores amount of students and teachers respectively

		for (vector<string> user : userDataBaseContent)														//	Loops through every user in the user database
		{
			if (user[3] == "0")																				//	Checks if the indexed user's useraccesslevel(user type) is student(0)
			{
				cout << "STUDENT\t" << user[0] << '\t' << user[1] << '\n';									//	Outputs that user is of type student, user's ID and name
				numOfStudents++;
			}
			else if (user[3] == "2")																		//	Checks if the indexed user's useraccesslevel(user type) is teacher(2)
			{
				cout << "TEACHER\t" << user[0] << '\t' << user[1] << '\n';									//	Outputs that user is of type student, user's ID and name
			}
		}

		if (numOfStudents > 0 || numOfTeachers > 0)															//	Checks if there more than 0 students or teachers
		{
			bool selectedExistingUser = false;																//	Stores whether the selected ID corresponds to an existing user

			do																								//	Loops atleast once
			{
				cout << "Please enter ID of user from list to assign a course.\n";
				cin >> selectedUserID;																		//	Stores input ID for user to assign a course

				for (vector<string> user : userDataBaseContent)												//	Loops through user's in the user database
				{
					if (user[0] == selectedUserID)															//	Checks if the indexed user's ID is the selectedUserID
					{
						selectedExistingUser = true;														//	Stores that the ID belongs to a valid user

						if (user[3] == "0" || user[3] == "2")												//	Checks if the user is of type student(0) or teacher(2)
						{
							selectedUserFile = selectedUserID;												//	Sets user filepath to selectedUserID
							selectedUserFile.append(".csv");												//	Adds filetype (.CSV)
							vector<vector<string>> selectedUserFileContent = ReadFile(selectedUserFile);	//	Reads the selectedUserFile and stores the content in the selectedUserFileContent matrix

							vector<vector<string>> courses = ReadFile("courses.csv");						//	Reads the courses file and stores the contents in the courses matrix

							if (!courses.empty())															//	Checks if the courses matrix is not empty
							{
								bool selectedValidCourse = false;											//	Stores if the user has selected an existing course (defaults to false)

								string selectedCourse;														//	Stores the user's input for desired course

								do																			//	Loops atleast once
								{
									for (vector<string> course : courses)									//	Loops through the courses in courses
									{
										cout << "\t" << course[0] << "\n";									//	Outputs the course name
									}

									cout << "Please enter name of course you want to assign " << selectedUserFileContent[1][0] << "\n";	//	Outputs instructions and the name of the selected user
									cin >> selectedCourse;													//	Stores the input for desired course

									for (vector<string> course : courses)									//	Loops through the courses in courses
									{
										if (course[0] == selectedCourse)									//	Checks if the indexed course is the selected course (desired course)
										{
											selectedValidCourse = true;									//	Stores that the selected course exists
										}
									}

									if (!selectedValidCourse)											//	Checks if the selected course does not exist
									{
										cout << "Warning! You selected an invalid course. Please check your spelling and try again.\n";
									}

									for (string userCourse : selectedUserFileContent[4])					//	Loops through the selected users already assigned courses
									{
										if (userCourse == selectedCourse)									//	Checks if the selected user already has the selected course
										{
											selectedValidCourse = false;									//	Stores that the input is invalid (since the user cannot enroll the same course twice)
											cout << "Warning! User already assigned to this course. Please select another.\n";
										}
									}
								} while (!selectedValidCourse);												//	Loops while the user has not selected a valid course

								if (selectedUserFileContent[4][0] == "placeholder")							//	Checks if the selected user's first course is a placeholder, thus not containing any real courses
								{
									selectedUserFileContent[4][0] = selectedCourse;							//	Replaces the placeholder with the desired course
								}
								else {																		//	Enters here if the user doesn't have a placeholder as the first course
									selectedUserFileContent[4].push_back(selectedCourse);					//	Adds the desired course to the selected user's courses
								}

								fstream changeUserFile(selectedUserFile, ios::out);							//	Opens an output filestream to the selected users filepath

								if (!changeUserFile.is_open())												//	Checks if the filestream failed to open
								{
									cout << "Warning! File couldn't be opened.\n";
									return TakeBackKey();													//	Waits until the user presses the back(left) arrow and then closes the menu
								}


								for (int i = 0; i < selectedUserFileContent.size(); i++)					//	Loops through the selectedUserFileContent matrix's rows
								{
									for (int j = 0; j < selectedUserFileContent[i].size(); j++)				//	Loops through the selectedUserFileContent matrix's rows' cells
									{
										changeUserFile << selectedUserFileContent[i][j];					//	Outputs the indexed cell of the selectedUserFileContent to the selected user's file
										if (j != selectedUserFileContent[i].size() - 1)						//	Checks if it isn't indexing the last cell of the row
										{
											changeUserFile << ",";											//	Adds a comma (,) to separate the cells
										}
									}

									if (i != selectedUserFileContent.size() - 1)							//	Checks if it isn't indexing the last row of the matrix
									{
										changeUserFile << "\n";												//	Outputs a linebreak to the file to separate rows
									}
								}

								changeUserFile.close();														//	Closes / dereferences the filestream

								if (user[3] == "0")															//	Checks if the selected user's userAccessLevel (user type) is student(0)
								{
									selectedCourse.append(".csv");											//	Adds filetype to selected course to make it a filepath

									fstream selectedCourseFile(selectedCourse, ios::app);					//	Opens an appending filestream

									if (!selectedCourseFile.is_open())										//	Checks if the filestream failed to open
									{
										cout << "Warning! File couldn't be opened.\n";
										return TakeBackKey();												//	Waits until the user presses the back(left) arrow and then closes the menu
									}

									selectedCourseFile << selectedUserFileContent[0][0] << "," << selectedUserFileContent[1][0] << "," << "D" << "\n";	//	Appends the selected user's ID, name and a default grade of D as well as a linebreak

									selectedCourseFile.close();												//	Closes / dereferences the filestream
								}

								cout << "Added user to course.\n";	

								return TakeBackKey();														//	Waits until the user presses the back(left) arrow and then closes the menu
							}
							else {																			//	Enters here if there are no existing courses
								cout << "Warning! No existing courses.\n";
								return TakeBackKey();														//	Waits until the user presses the back(left) arrow and then closes the menu
							}
						}
						else {																				//	Enters here if the user has chosen a user of any other tpye than student(0) or teacher(2)
							cout << "Warning! You selected an invalid account-type. Try again.\n";
							return TakeBackKey();															//	Waits until the user presses the back(left) arrow and then closes the menu
						}
					}
				}

				if (!selectedExistingUser)																	//	Enters here if the user hasn't selected an existing ID/user
				{
					cout << "Warning! Invalid ID. Try again.\n";
					TakeBackKey();																			//	Waits until the user presses the back(left) arrow and then continues with the loop
				}
			} while (!selectedExistingUser);																//	Loops while the user hasn't selected an existing user
		}
		else {																								//	Enters here if there are no student or teacher accounts in the user database
			cout << "Warning! No student or teacher accounts found on record.\n";
		}

		return TakeBackKey();																				//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "assignGuardian")												//	Enters here and lets admin assign a student a guardian
	{
		vector<vector<string>> userDataBaseContent = ReadFile("userDataBase.csv");		//	Reads the user database and stores the contents in the userDataBaseContent matrix

		string selectedStudentID, selectedGuardianID;									//	selectedStudentID and selectedGuardianID are used to select a student and guardian respectively

		int numOfStudents = 0, numOfGuardians = 0;										//	Stores the amount of students and guardians respectively

		for (vector<string> user : userDataBaseContent)									//	Loops through the users in the user database
		{
			if (user[3] == "0")															//	Checks if the user has userAccessLevel (user type) student(0)
			{
				numOfStudents++;														//	Increments number of students found
				cout << '\t' << user[0] << '\t' << user[1] << '\n';						//	Outputs student's id and name
			}
			else if (user[3] == "1")													//	Checks if the user has userAccessLevel (user type) guardian(1)
			{
				numOfGuardians++;														//	Increments number of guardians
			}
		}

		if (numOfStudents > 0 && numOfGuardians > 0)									//	Checks if there are more than 0 students and guardians
		{
			cout << "Please enter ID of student from list to assign a guardian.\n";
			cin >> selectedStudentID;													//	Stores the input selected ID

			for (vector<string> user : userDataBaseContent)								//	Loops through the users in the user database
			{
				if (user[0] == selectedStudentID)										//	Checks if the indexed user's ID is equal to the selected student ID
				{
					if (user[3] == "0")													//	Checks if the indexed user has userAccessLevel (user type) student(0)
					{
						string selectedGuardianFile, selectedStudentFile = selectedStudentID;	//	selectedGuardianFile and selectedStudentFile are used to store filepaths for the desired student and guardian respectively
						selectedStudentFile.append(".csv");								//	Adds file type (.CSV) to the student filepath

						vector<vector<string>> selectedStudentFileContent = ReadFile(selectedStudentFile);	//	Reads the student file and stores the contents in the selectedStudentFileContent matrix

						if (!selectedStudentFileContent.empty())						//	Checks if the student file isn't empty
						{
							for (vector<string> _user : userDataBaseContent)			//	Loops through the users in the user database
							{
								if (_user[3] == "1")									//	Checks if the indexed user has userAccessLevel (user type) guardian(1)
								{
									cout << '\t' << _user[0] << '\t' << _user[1] << '\n';	//	Outputs indexed user's ID and name
								}
							}

							cout << "Please enter ID of guardian from list to assign a guardian.\n";
							cin >> selectedGuardianID;									//	Stores the input selected guardian ID

							for (vector<string> _user : userDataBaseContent)			//	Loops through the users in the user database
							{
								if (_user[0] == selectedGuardianID)						//	Checks if the indexed user's ID is equal to the selectedGuardianID
								{
									if (_user[3] == "1")								//	Checks if the indexed user has userAccessLevel (user type) guardian(1)
									{
										selectedGuardianFile = selectedGuardianID;		//	Sets the guardian filepath to the selected guardian ID
										selectedGuardianFile.append(".csv");			//	Adds file type (.CSV)

										vector<vector<string>> selectedGuardianFileContent = ReadFile(selectedGuardianFile);	//	Reads the guardian file and stores the content in the selectedGuardianFileContent matrix

										if (selectedGuardianFileContent[4][0] == "placeholder")									//	Checks if the selected guardians first child is a placeholder
										{
											selectedGuardianFileContent[4][0] = selectedStudentID;								//	Replaces the placeholder with the selectedStudentID
										}
										else {																					//	Enters here if the first child of the guardian isn't a placeholder
											for (string guardiansChild : selectedGuardianFileContent[4])						//	Loops through the guardians children
											{
												if (guardiansChild == selectedStudentID)										//	Checks if the indexed child is equal to the selected student
												{
													cout << "Warning! The chosen child has already been assigned to this guardian.\n";
													return TakeBackKey();														//	Waits until the user presses the back(left) arrow and then closes the menu
												}
											}
											selectedGuardianFileContent[4].push_back(selectedStudentID);						//	Appends the selected child to the guardian
										}

										if (selectedStudentFileContent[6][0] == "placeholder")									//	Checks if the selected student's first guardian is a placeholder
										{
											selectedStudentFileContent[6][0] = selectedGuardianID;								//	Replaces the placeholder with the selected guardian ID
										}
										else {																					//	Enters here if the first guardian of the student isn't a placeholder
											selectedStudentFileContent[6].push_back(selectedGuardianID);						//	Appends the selected guardian
										}

										EditFile(selectedStudentFileContent, selectedStudentFile);								//	Overwrites the selected student's userfile with the edited selectedStudentFileContent matrix
										EditFile(selectedGuardianFileContent, selectedGuardianFile);							//	Overwrites the selected guardian's userfile with the edited selectedGuardianFileContent matrix

										cout << "Guardian assigned.\n";
										return TakeBackKey();																	//	Waits until the user presses the back(left) arrow and then closes the menu
									}
									else {																						//	Enters here if the selected guardian ID isn't a guardian account
										cout << "Selected user is not a guardian account.\n";
									}
								}
							}
						}
					}
					else {																										//	Enters here if the selected student ID isn't a student account
						cout << "Selected user is not a student account.\n";
					}
				}
			}
		}
		else if (numOfStudents <= 0)																							//	Enters here if there are no student accounts in the user database
		{
			cout << "Warning! No student accounts found on record.\n";
		}
		else if (numOfGuardians <= 0)																							//	Enters here if there are no guardian accounts in the user database
		{
			cout << "Warning! No guardian accounts found on record.\n";
		}

		return TakeBackKey();																									//	Waits until the user presses the back(left) arrow and then closes the menu
	}
	else if (inputType == "studentSupportReport")						//	Enters here and lets admin see which students are struggling as well as any guardians (and the guardians contact details)
	{
		vector<vector<string>> courses = ReadFile("courses.csv");		//	Reads the courses file and stores the contents in the courses matrix

		for (vector<string> course : courses)							//	Loops through the courses in the courses matrix
		{
			string courseNameUpper = course[0];							//	Stores the course name in a temporary string
			courseNameUpper[0] = std::toupper(courseNameUpper[0]);		//	Capitalizes the first letter of the temporary string
			cout << courseNameUpper << '\n';							//	Outputs the temporary string

			string courseFile = course[0];								//	Stores the filepath to the course as the name of the course
			courseFile.append(".csv");									//	Adds file type (.CSV)

			vector<vector<string>> courseFileContent = ReadFile(courseFile);	//	Reads the course file and stores the contents in the courseFileContent matrix

			for (vector<string> student : courseFileContent)			//	Loops through the students in the courseFileContent matrix
			{
				if (student[2] == "F" || student[2] == "D")				//	Checks if the grade of the indexed student is either D or F
				{
					string studentFile = student[0];					//	Stores the filepath to the student as their ID
					studentFile.append(".csv");							//	Adds file type (.CSV)
					vector<vector<string>> studentFileContent = ReadFile(studentFile);	//	Reads the student file and stores the contents in the studentFileContent matrix

					cout << student[0] << '\t' << student[1] << '\t' << student[2] << '\t' << " : May require extra support.\n";	//	Outputs the student's ID, name and grade, as well as a message that they may need support
					if (studentFileContent[6][0] != "placeholder")		//	Checks if the indexed student's first guardian isn't a placeholder
					{
						for (string guardian : studentFileContent[6])	//	Loops through the guardians of the indexed student
						{
							string guardianFile = guardian;				//	Stores the guardian filepath as the indexed guardian's ID
							guardianFile.append(".csv");				//	Adds file type (.CSV)
							vector<vector<string>> guardianFileContent = ReadFile(guardianFile);	//	Reads the guardians file and stores the contents in the guardianFileContent matrix

							cout << guardianFileContent[1][0] << '\t';	//	Outputs the guardians name

							if (guardianFileContent[5][0] != "placeholder")	//	Checks if the indexed guardians contact details aren't a placeholder
							{
								for (string contactMethod : guardianFileContent[5])	//	Loops through their contact methods
								{
									cout << contactMethod << '\t';		//	Outputs the contact method
								}
							}
							else {										//	Enters here if the guardian has no contact methods in their account
								cout << "No contact method found";
							}
							cout << '\n';								//	Outputs a linebreak
						}
					}
					else {												//	Enters here if the indexed student has no guardians assigned
						cout << "No guardian assigned" << '\t';
					}
					cout << '\n';										//	Outputs a linebreak
				}
			}
		}

		return TakeBackKey();											//	Waits until the user has pressed the back(left) arrow and then closes the menu
	}
	else {																//	Enters here if the menu csv file does not specify an input type
		cout << "ERROR! CSV menu file does not specify valid input type.\n";
		return TakeBackKey();											//	Waits until the user presses the back(left) arrow and then closes the menu
	}
}