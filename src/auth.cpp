#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "auth"
using namespace std;

string getpass() {
	string pass;
	char ch;
	while (ch = _getwch() != '\r') {
		if (ch == (char)10)
			cout << (char)251;
		else {
			pass += ch;
			cout << (char)250;
		}
	}
	return pass;
}

int auth() {
	ifstream fin("users.bin", ios::binary | ios::in);
	bool again;
	do {
		again = false;

		string login, pass;
		cout << "login: ";
		getline(cin, login);
		cout << "pass: ";
		getline(cin, pass);
		string user, user_input = login + ":" + pass + ":";

		if (fin.is_open()) {
			while (!fin.eof()) {
				getline(fin, user);
				if (user.substr(0, user.length() - 2) == user_input) {
					fin.close();
					system("cls");

					cout << "hello, " << login << "\n" << endl;
					cout << "press any key...";
					_getwch();

					return user.at(user.length() - 2) - '0';
				}
			}
			system("cls");
			cout << "Incorrect login or password\n" << endl;
			cout << "press any key...";
			_getwch();

			again = true;
			fin.clear();
			fin.seekg(0, ios::beg);
			system("cls");
		}
		else {
			cout << "File is not avalible" << endl;
			abort();
		}
	} while (again);
}
