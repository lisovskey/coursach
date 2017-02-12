/*
* RitZEED inc.
*/

#include <iostream>
#include <string>
#include <sstream>
#include "constants"
#include "drawer"
#include "console"
#include "stringer"
#include "input"
using namespace std;

size_t getPositiveNumber(size_t y) {
	string line;
	size_t number;

	while (true) {
		getline(cin, line);
		cin.clear();
		stringstream ss(line);
		if (ss >> number && line.find('-') == string::npos)
			if (ss.eof())
				return number;

		clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
		drawPreCentered("Try again: ", y);
	}
}

bool getBoolean(size_t y) {
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

		clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
		drawPreCentered("Try again: ", y);
	}
}
