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
	/// Takes arrow key input to navigate the menu options
	/// </summary>
	/// <param name="menuFile">File name for the menu to navigate</param>
	/// <returns>Returns whether to keep current menu open or not</returns>
	bool TakeArrowKeys(string menuFile, int id);
	bool TakeBackKey();
	bool UserLogin();
	void CreateUserAccount();
	void EditGrades(string courseName);
	vector<vector<string>> ReadFile(string fileName);

public:
	/// <summary>
	/// Outputs input instructions, menu options and highlights the user's selected option
	/// </summary>
	/// <param name="menuFile">Menu csv file adress to output</param>
	/// <returns>Returns whether to keep current menu open or not</returns>
	bool PrintMenu(string menuFile, string inputType, int id);
};
