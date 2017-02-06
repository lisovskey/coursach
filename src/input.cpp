/*
* RitZEED inc.
*/

#include <iostream>
#include <string>
#include <sstream>
#include "stringer"
#include "input"
using namespace std;

size_t getNumber() {
	string line;
	size_t number;

	while (true) {
		getline(cin, line);
		cin.clear();
		stringstream ss(line);
		if (ss >> number && line.find('-') == string::npos)
			if (ss.eof())
				return number;

		cout << "Try again: ";
	}
}

bool getBoolean() {
	string line;
	while (true) {
		getline(cin, line);
		cin.clear();
		line = lower(line);

		if (line == "yes" || line == "y" ||
			line == "true" || line == "1") {
			return true;
		}
		if (line == "no" || line == "n" ||
			line == "false" || line == "0") {
			return false;
		}

		cout << "Try again: ";
	}
}
