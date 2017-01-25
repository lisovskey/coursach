/*
* RitZEED inc.
*/

#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include "input"
using namespace std;

unsigned int getNumber() {
	string line;
	unsigned int number;

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
	bool boolean;

	while (true) {
		getline(cin, line);
		cin.clear();
		for (unsigned short i = 0; i < line.length(); i++)
			line[i] = tolower(line[i]);

		if (line == "yes" || line == "y" || line == "true")
			return true;
		if (line == "no" || line == "n" || line == "false")
			return false;

		stringstream ss(line);
		if (ss >> boolean)
			if (ss.eof())
				return boolean;

		cout << "Try again: ";
	}
}