/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "constants"
#include "auth"
using namespace std;

typedef struct {
	char	login[20];
	char	pass[20];
	bool	admin;
} account;

bool auth() {
	account user, input;
	ifstream fin(ACCS, ios::binary | ios::in);
	if (fin.is_open()) {
		while (true) {
			system("cls");
			cout << "login: ";
			cin >> input.login;
			cout << "pass: ";
			cin >> input.pass;

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