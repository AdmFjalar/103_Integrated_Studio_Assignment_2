// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "UI_Menu.h";

int main()
{
	UI_Menu* uiMenu = new UI_Menu();							//	Creates a new instance of a UI_Menu

	while (uiMenu->PrintMenu("mainmenu.csv", "arrow_keys", 0));	//	Opens a menu from file mainmenu.csv with input type arrow_keys and temporary user ID of 0

	delete(uiMenu);												//	Deletes uiMenu
	uiMenu = nullptr;											//	Sets uiMenu to a null pointer
}