#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class UI_Menu
{
private:
	int menuOptionIndex = 0;	//Used navigate through menu indexes

	/// <summary>
	/// Uses arrow keys to navigate a menu, changing index when using up/down arrow keys, exiting menu with back(left) key, and loading a new menu when using right arrow key
	/// </summary>
	/// <param name="menuFile">File path of the current menufile</param>
	/// <param name="id">ID of the current user</param>
	/// <returns>Returns whether to stay in the current menu or not</returns>
	bool TakeArrowKeys(string menuFile, int id);
	
	/// <summary>
	/// Waits until the user has pressed the back(left) arrow key and then returns a false value (used to signal that the user does not want to stay in their current menu)
	/// </summary>
	/// <returns>Returns false when the back(left) arrow key has been pressed</returns>
	bool TakeBackKey();
	
	/// <summary>
	/// Outputs a login screen, validates input against user database and gives user 3 attempts
	/// </summary>
	/// <returns>Whether the login attempt succeeded or not</returns>
	bool UserLogin();
	
	/// <summary>
	/// Outputs a signup screen, assigns ID and writes the created user to the user database and a new user file
	/// </summary>
	void CreateUserAccount();
	
	/// <summary>
	/// Let's the user edit the grades in the loaded course
	/// </summary>
	/// <param name="courseName">The course to edit grades in (name, not filepath, i.e. english not english.csv)</param>
	void EditGrades(string courseName);
	
	/// <summary>
	/// Loads a file and returns the contents in a matrix
	/// </summary>
	/// <param name="fileName">Filename of the file to load and read contents from</param>
	/// <returns>Returns a matrix with the filecontents</returns>
	vector<vector<string>> ReadFile(string fileName);
	
	/// <summary>
	/// Opens a filestream and edits the file
	/// </summary>
	/// <param name="fileContent">The matrix that will be fed into the file</param>
	/// <param name="filePath">Filepath to the file to edit (fileName.csv)</param>
	void EditFile(vector<vector<string>> fileContent, string filePath, bool addLineBreakOnLastLine);

public:
	/// <summary>
	/// Outputs the loaded menu
	/// </summary>
	/// <param name="menuFile">Filepath (either a .csv file or No_Menu_File)</param>
	/// <param name="inputType">What kind of input the menu should use (arrow_keys, none, etc)</param>
	/// <param name="id">ID of the user (use 0 until a user has been logged in)</param>
	/// <returns>Returns whether to stay in the menu or not</returns>
	bool PrintMenu(string menuFile, string inputType, int id);
};
