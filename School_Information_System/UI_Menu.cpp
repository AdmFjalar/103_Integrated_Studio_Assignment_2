#include "UI_Menu.h"
#include <iostream>;
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <conio.h>

using std::ofstream;
using std::ifstream;
using std::fstream;
using std::stringstream;
using std::ios;
using std::string;
using std::vector;
using std::cout;
using std::cin;

bool UI_Menu::TakeInput(string menuFile)
{
    fstream fileStream;

    fileStream.open(menuFile, ios::in | ios::beg);

    if (!fileStream.is_open())
    {
        cout << "Warning! File could not be opened.\n";
        return false;
    }

    string line, word;
    vector<string> row;
    vector<vector<string>> fileContent;

    while (getline(fileStream, line, '\n'))
    {
        row.clear();
        stringstream stream(line);
        while (getline(stream, word, ','))
        {
            row.push_back(word);
        }
        fileContent.push_back(row);
    }

    fileStream.close();

    switch (_getch()) {
        case 72: //Up arrow
        {
            menuOptionIndex--;
            if (menuOptionIndex < 0)
            {
                menuOptionIndex = (fileContent.size() - 1);
            }
            return true;
            break;
        }
        case 80: //Down arrow
        {
            menuOptionIndex++;
            if (menuOptionIndex >= fileContent.size())
            {
                menuOptionIndex = 0;
            }
            return true;
            break;
        }
        case 77: //Right arrow
        {
            UI_Menu* nextMenu = new UI_Menu();
            if (fileContent[menuOptionIndex].size() > 1)
            {
                while (nextMenu->PrintMenu(fileContent[menuOptionIndex][1]));
            }
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

bool UI_Menu::PrintMenu(string menuFile)
{
    fstream fileStream;

    fileStream.open(menuFile, ios::in | ios::beg);

    if (!fileStream.is_open())
    {
        cout << "Warning! File could not be opened.\n";
        return false;
    }

    string line, word;
    vector<string> row;
    vector<vector<string>> fileContent;

    while (getline(fileStream, line, '\n'))
    {
        row.clear();
        stringstream stream(line);
        while (getline(stream, word, ','))
        {
            row.push_back(word);
        }
        fileContent.push_back(row);
    }

    fileStream.close();

        cout << "-----------------------------------------\n";
        for (int i = 0; i < fileContent.size(); i++)
        {
            if (i == menuOptionIndex)
            {
                cout << "* ";
            }
            cout << fileContent[i][0];
            cout << '\n';
        }
        cout << "-----------------------------------------\n";
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n";
        return TakeInput(menuFile);
}
