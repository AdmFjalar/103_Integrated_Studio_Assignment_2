#pragma once
#include <string>

using std::string;

class UI_Menu
{
	bool enteredMenu = false;
	bool exitMenu = false;
	int menuOptionIndex = 0;

	bool TakeInput(string menuFile);

public:
	bool PrintMenu(string menuFile);
};

