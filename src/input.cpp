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

string getPass(size_t size) {
	// Скрытие ввода пароля
	char* result = new char[size];
	memset(result, '\0', sizeof(char) * size);

	DWORD mode, count;
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleMode(ih, &mode))
		throw runtime_error(
			"getPass: You must be connected to a console\n"
		);
	SetConsoleMode(ih, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	char c;
	char symbol[1];
	symbol[0] = (char)250;
	size_t length = 0;
	while (ReadConsoleA(ih, &c, 1, &count, NULL) && (c != '\r') && (c != '\n')) {
		if (c == '\b' && length != 0) {
			WriteConsoleA(oh, "\b \b", 3, &count, NULL);
			if (length-- < size) {
				result[length] = '\0';
			}
		}
		else {
			WriteConsoleA(oh, symbol, 1, &count, NULL);
			if (length++ < size - 1) {
				result[length - 1] = c;
			}
		}
	}
	SetConsoleMode(ih, mode);

	string pass = result;
	delete[] result;
	return pass;
}
