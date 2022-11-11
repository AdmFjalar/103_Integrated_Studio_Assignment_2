#pragma once
#include <string>
#include <vector>

using std::string;
using std::vector;

class School
{
	string schoolName;
	string contactDetails;
	vector<string> classes;
	vector<vector<string>> events;
	vector<vector<string>> terms;
};