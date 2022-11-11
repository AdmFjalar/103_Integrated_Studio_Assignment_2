// school structure.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using std::cout;
using std::cin;
using std::fstream;
using std::ios;
using std::string;
using std::vector;
using std::stringstream;


// file writing
// -------------------------------------------------------------------------------------------------

void fileWrite(string& fileName) {
   
    fstream file(fileName, ios::out | ios::app);
    if (!file.is_open()) {
        cout << "Warining No file is open to write to\n";
        return;
    }

    // variables
    string studentName, studentGender, parentName, course, courseA, courseB, courseC, grades;
    int age, term;

    // user prompt
    cout << "Enter details:\n\n";
    cout << "name of student: ";
    cin >> studentName;
    cout << "\nGender of student (M or F)";
    cin >> studentGender;
    cout << "\nName of parent: ";
    cin >> parentName;
    cout << "\nAge of student: ";
    cin >> age;
    cout << "\nEnter course: ";
    cin >> course;
    cout << "\nEnter course A: ";
    cin >> courseA;
    cout << "\nEnter course B: ";
    cin >> courseB;
    cout << "\nEnter course C: ";
    cin >> courseC;
    cout << "\nGrade of student: ";
    cin >> grades; 
    cout << "\nEnter Term Number(eg: 1, 2, 3 or 4)";
    cin >> term;

    cout << "\n\n";

    // send the information to the file
    file << studentName << "\n" << studentGender << "\n" << parentName << "\n" << age << "\n" << course << "\n" << grades << "\n" << term << "\n" << courseA << "," << courseB << "," << courseC << "\n";
   
    

    file.close();
    if (!file.is_open()) {
        cout << "file has been successfully closed\n\n";
    }
}

// read file
// -------------------------------------------------------------------------------------------------

vector<vector<string>> fileRead(string& fileName) {

    string line, word;
    vector<string> row;
    vector <vector<string>> fileContent;

    // create an instance of the file stream
    fstream file(fileName, ios::in | ios::beg);
    // make our check to see if the file is open
    if (!file.is_open()) {
        cout << "Waring please open a file first.\n\n";
        return fileContent; // return fileContent vector will be empty at this point
    }
    //iterate through our file
    while (std::getline(file, line, '\n')) {
        row.clear();
        stringstream stream(line);

        while (getline(stream, word, ',')) {
            row.push_back(word);
        }
        fileContent.push_back(row);
    }

    // need to loop through our created vector so we can out put the data
    for (vector<string> i_row : fileContent) {
        // loop through eace row and out put word
        for (string i_word : i_row) {
            cout << i_word << "\t"; // output word
        }
        cout << "\n";
    }
    // close the file
    file.close();
    // check to make sure its not open
    if (!file.is_open()) {
        cout << "File successfully closed\n\n";
    }
    // return file content
    return fileContent;
}


// row clear / deleting data
// -------------------------------------------------------------------------------------------------

void rowClear(string& fileName) {
    // create a new file 
    string tempFileName = "temp" + fileName;
    // create pointers give a reference to the file
    char* fileNamePtr = &fileName[0];
    char* tempFileNamePtr = &tempFileName[0];

    // creating an instance of the file stream
    fstream fileIn(fileName, ios::in);
    if (!fileIn.is_open()) {
        cout << "Warning no file is open\n\n"; // out putting warning
        return; // end execution of the function
    }

    fstream fileOut(tempFileName, ios::out); // initialised content assigned from a read file
    vector<vector<string>> fileContent = fileRead(fileName);

    //check to see if file has content
    if (fileContent.size() == 0) {
        cout << "Warning file contains no data\n\n";
        fileIn.close(); // close input file
        fileOut.close(); // close the out put file
        remove(tempFileNamePtr); // removing the output file
        return; // end execution of function
    }

    // if prior checks are cleared, creator a vector
    vector<vector<string>> replacementContent;
    string delTarget; // string to take user input, remove an entry

    cout << "\n\nPlease enter the name of the user you wish to delete\n\n";
    for (vector<string> i_row : fileContent) {
        cout << i_row.at(0) << "\t"; // for everythin gin column 0 output it |  i_row.at(0) is the same as i_row[0]
    }
    cout << "Enter name: ";
    cin >> delTarget;
    cout << "\n";

    // loop[ through content which we will push to our new vector
    for (vector<string> row : fileContent) {
        if (row.at(0) == delTarget) {
            cout << "continue through\n\n";
            continue;
        }
        else {
            replacementContent.push_back(row); // push back to our replacment content vector
        }
    }

    // loop through replacement content display new data
    cout << "Data witin new file:\n";
    for (vector<string> row : replacementContent) {
        for (string word : row) {
            cout << word << "\t";
        }
        cout << "\n";
    }
    // output everything into new temp file
    for (vector<string> row : replacementContent) {
        fileOut << row.at(0) << "," << row.at(1) << "," << row.at(2) << "\n";
    }

    // close files
    fileIn.close();
    fileOut.close();
    remove(fileNamePtr); // deleting original file
    rename(tempFileNamePtr, fileNamePtr);
}

// delete file
// -------------------------------------------------------------------------------------------------

void fileDelete(string& fileName) {
  
    fstream file(fileName, ios::out | ios::app);
    if (!file.is_open()) {
        cout << "Warining No file is open to write to\n";
        return;
    }

    cout << "Please input the name of the file you wish to delete: " << '\n';
    cin >> fileName;

    file.open("fileName.csv", std::ofstream::out | std::ofstream::trunc);
    file.close();

    if (!file.is_open()) {
        cout << "File successfully deleted\n\n";
    }
}

// create/open file
// -------------------------------------------------------------------------------------------------

bool fileOpen(string& f) {

    cout << "What is the name of the file you want to create/\n";
    cout << "For file naming conventions use camel casing eg: nameOfFile.\n";
    cout << "You can make files for Classes, Terms times, School Infomation, Assigning Classes to students and teacher, etc.\n";
    cout << "Enter file name: ";
    cin >> f;

    f.append(".csv");
    cout << "The file: " << f << " has been created.\n\n";

    fstream file(f, ios::out | ios::in | ios::app);

    if (!file.is_open()) {
        cout << "Warning file failed to open\n";
        return false;
    }
    else {
        if (!file.is_open()) {
            cout << "file has been opened\n\n";
        }
        return true;
    }
}

// main function
// -------------------------------------------------------------------------------------------------

int main()
{
    string fileName;
    bool b_fileOpen = false;

    int adminChoice;

    do {
        cout << "What Function Would You Like To Preform\n\n";
        cout << "1. Create file\n2. write to a file\n3. Read from a file\n4. Delete from a file\n5. Delete a file\n6. Exit\n";
        cout << "\nEnter choice: ";
        cin >> adminChoice;
        cout << "\n\n";

        switch (adminChoice)
        {
        case 1:
            // creating a file
            b_fileOpen = fileOpen(fileName);
            break;
        case 2:
            // write to a file
            if (!b_fileOpen) {
                cout << "Warning: Please open a file first\n\n";
            }
            else {
                fileWrite(fileName);
            }
            break;
        case 3:
            // read from a file
            if (!b_fileOpen) {
                cout << "Warning: Please open a file first.\n\n";
            }
            else {
                fileRead(fileName);
            }
            break;
        case 4:
            // deleting from a file
            rowClear(fileName);
            break;
        
        case 5:
            // delete file
            fileDelete(fileName);
            break;
        case 6:
            cout << "thanks for using xyz\n\n";
            break;

        }

    } while (adminChoice != 5);


}