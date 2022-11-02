#include "UI_Menu.h"
#include "string"
#include <iostream>;
#include <conio.h>

using std::string;
using std::cout;

bool UI_Menu::TakeInput(int amountOfOptions)
{
    switch (_getch()) {
        case 72: //Up arrow
        {
            menuOptionIndex--;
            if (menuOptionIndex < 0)
            {
                menuOptionIndex = (amountOfOptions - 1);
            }
            return true;
            break;
        }
        case 80: //Down arrow
        {
            menuOptionIndex++;
            if (menuOptionIndex >= amountOfOptions)
            {
                menuOptionIndex = 0;
            }
            return true;
            break;
        }
        case 77: //Right arrow
        {
            UI_Menu* uiMenu = new UI_Menu();

            //while(uiMenu->PrintMenu())
            return true;
            break;
        }
        case 75: //Left arrow
        {
            return false;
            break;
        }
        default:
        {
            break;
        }
    }
}

bool UI_Menu::PrintMenu(string* menuOptions, int amountOfOptions)
{
        cout << "-----------------------------------------\n";
        for (int i = 0; i < amountOfOptions; i++)
        {
            if (i == menuOptionIndex)
            {
                cout << "* ";
            }
            cout << *(menuOptions + i);
            cout << '\n';
        }
        cout << "-----------------------------------------\n";
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        return TakeInput(amountOfOptions);
}
