// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include "UI_Menu.h";
#include "Menu_Item.h";

using std::string;

int main()
{
	UI_Menu* uiMenu = new UI_Menu();

	

	string studentMainMenu[4] = {
		"Check grades",
		"Timetable",
		"Learning progress",
		"Classes"
	};

	//UI_Menu studentMainMenuOptions[4] = 
	//{
	//	
	//}

	while(uiMenu->PrintMenu(studentMainMenu, 4));
}