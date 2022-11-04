#include "Base_Member.h"
#include <string>

using std::string;

Base_Member::Base_Member() {

}

Base_Member::Base_Member(int _age, int _id, string _name, string _password, int _userAccessLevel) {
	age = _age;
	id = _id;
	name = _name;
	password = _password;
	userAccessLevel = _userAccessLevel;
}

