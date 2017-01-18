/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "auth"
using namespace std;

int auth() {
	ifstream fin("users.bin", ios::binary | ios::in);
	while (true) {
		if (fin.is_open()) {
			system("cls");
			string login, pass;
			cout << "login: ";
			getline(cin, login);
			cout << "pass: ";
			getline(cin, pass);
			string user, user_input = login + ":" + pass + ":";

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

			fin.clear();
			fin.seekg(0, ios::beg);
		}
		else {
			cout << "File is not avalible" << endl;
			abort();
		}
	}
}
