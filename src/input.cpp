#include <iostream>
#include <string>
#include <sstream>
using namespace std;

unsigned int getNumber() {
	string line;
	unsigned int number;
	cin.ignore();

	while (true) {
		getline(cin, line);
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
	cin.ignore();

	while (true) {
		getline(cin, line);

		if (line == "yes" || line == "true")
			return true;
		else if (line == "no" || line == "false")
			return false;

		stringstream ss(line);
		if (ss >> boolean)
			if (ss.eof())
				return boolean;

		cout << "Try again: ";
	}
}