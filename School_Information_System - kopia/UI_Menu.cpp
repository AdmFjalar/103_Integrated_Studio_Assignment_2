#include "UI_Menu.h"
#include "string"
#include <iostream>;
#include <conio.h>

using std::string;
using std::cout;

void UI_Menu::TakeInput(int amountOfOptions)
{
    switch (_getch()) {
        case 72:
        {
            menuOptionIndex--;
            if (menuOptionIndex < 0)
            {
                menuOptionIndex = (amountOfOptions - 1);
            }
            break;
        }
        case 80:
        {
            menuOptionIndex++;
            if (menuOptionIndex >= amountOfOptions)
            {
                menuOptionIndex = 0;
            }
            break;
        }
        case 77:
        {
            break;
        }
        case 75:
        {
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
        TakeInput(amountOfOptions);

    return true;
}
