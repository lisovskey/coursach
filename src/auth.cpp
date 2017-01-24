/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <windows.h>
#include "constants"
#include "auth"
using namespace std;

typedef struct {
	char	login[20];
	char	pass[20];
	bool	admin;
} account;

string getPass() {
	string result;

	DWORD mode, count;
	HANDLE ih = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE oh = GetStdHandle(STD_OUTPUT_HANDLE);
	if (!GetConsoleMode(ih, &mode))
		throw runtime_error(
			"getPass: You must be connected to a console to use this program.\n"
		);
	SetConsoleMode(ih, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT));

	char c;
	char symbol[1];
	symbol[0] = (char)250;
	while (ReadConsoleA(ih, &c, 1, &count, NULL) && (c != '\r') && (c != '\n')) {
		if (c == '\b') {
			if (result.length()) {
				WriteConsoleA(oh, "\b \b", 3, &count, NULL);
				result.erase(result.end() - 1);
			}
		}
		else {
			WriteConsoleA(oh, symbol, 1, &count, NULL);
			result.push_back(c);
		}
	}
	SetConsoleMode(ih, mode);

	return result;
}

bool auth() {
	account user, input;
	ifstream fin(ACCS, ios::binary | ios::in);
	if (fin.is_open()) {
		while (true) {
			system("cls");
			cout << "login: ";
			cin >> input.login;
			cout << "pass: ";
			strcpy_s(input.pass, getPass().c_str());
			cin.clear();
			cin.ignore(10000, '\n');

			while (!fin.eof()) {
				fin.read((char*)&user, sizeof(account));
				if (strcmp(input.login, user.login) == 0 && 
					strcmp(input.pass, user.pass) == 0) {
					fin.close();
					system("cls");
					cout << "hello, " << user.login << "\n" << endl;
					cout << "press any key...";
					_getwch();

					return user.admin;
				}
			}
			system("cls");
			cout << "Incorrect login or password\n" << endl;
			cout << "press any key...";
			_getwch();

			fin.clear();
			fin.seekg(0, ios::beg);
		}
	}
	else {
		cout << "File is not avalible" << endl;
		abort();
	}
}