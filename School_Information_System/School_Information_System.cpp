// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "UI_Menu.h";

using std::string;

int main()
{
	// testing push
	UI_Menu* uiMenu = new UI_Menu();

	string studentOptions[4] = {
		"Check grades",
		"Timetable",
		"Learning progress",
		"Classes"
	};

	while(uiMenu->PrintMenu(studentOptions, 4));
}