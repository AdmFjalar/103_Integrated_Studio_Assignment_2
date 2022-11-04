// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "UI_Menu.h";
#include "Base_Member.h";

using std::cout;
using std::cin;
using std::string;

int main()
{
	Base_Member* base_Member = new Base_Member();
	
	UI_Menu* uiMenu = new UI_Menu();

	string studentOptions[4] = {
		"Check grades",
		"Timetable",
		"Learning progress",
		"Classes"
	};

	while(uiMenu->PrintMenu(studentOptions, 4));
}