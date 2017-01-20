/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "auth"
using namespace std;

struct account {
	char	login[21];
	char	pass[21];
	bool	admin;
};

bool auth() {
	account user, input;
	ifstream fin("users.bin", ios::binary | ios::in);
	if (fin.is_open()) {
		while (true) {
			system("cls");
			cout << "login: ";
			cin >> input.login;
			cout << "pass: ";
			cin >> input.pass;

			while (!fin.eof()) {
				fin.read((char*)&user, sizeof(user));
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