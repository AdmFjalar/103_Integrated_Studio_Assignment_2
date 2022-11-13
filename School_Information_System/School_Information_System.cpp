// School_Information_System.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "UI_Menu.h";
#include "School.h";
#include "Base_Member.h";

using std::fstream;
using std::stringstream;
using std::ios;
using std::string;
using std::vector;
using std::cout;
using std::cin;

void EditGrades(string courseName);

int main()
{
	//EditGrades("english.csv");

	UI_Menu* uiMenu = new UI_Menu();

	while (uiMenu->PrintMenu("mainmenu.csv", "arrow_keys", 0));
}