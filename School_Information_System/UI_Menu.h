#pragma once
#include <string>

using std::string;

class UI_Menu
{
	bool enteredMenu = false;
	bool exitMenu = false;
	int menuOptionIndex = 0;

	bool TakeInput(int amountOfOptions);

public:
	bool PrintMenu(string* menuOptions, int amountOfOptions);
};

