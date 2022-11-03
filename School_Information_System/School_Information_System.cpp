// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "UI_Menu.h";

using std::ofstream;
using std::ifstream;
using std::fstream;
using std::stringstream;
using std::ios;
using std::string;
using std::vector;
using std::cout;
using std::cin;

int main()
{
	UI_Menu* uiMenu = new UI_Menu();

	//UI_Menu studentMainMenuOptions[4] =
	//{
	//
	//}

	while (uiMenu->PrintMenu("mainmenu.csv"));
}