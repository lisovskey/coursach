/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <windows.h>
#include "constants"
#include "input"
#include "drawer"
#include "account"
using namespace std;

typedef struct {
	unsigned short	id;
	char			login[20];
	char			pass[20];
	bool			admin;
} account;

vector<account> accounts;

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

bool getAccounts() {
	ifstream fin(ACCLIST, ios::binary | ios::in);
	if (fin.is_open()) {
		account tmp;
		while (fin.peek() != EOF) {
			fin.read((char*)&tmp, sizeof(account));
			accounts.push_back(tmp);
		}
	}
	else {
		cout << "File is not avalible" << endl;
		return false;
	}
	fin.close();
	return true;
}

bool auth() {
	if (!getAccounts()) abort();

	account input;
	ifstream fin(ACCLIST, ios::binary | ios::in);
	while (true) {
		system("cls");
		cout << "login: ";
		cin >> input.login;
		cout << "pass: ";
		strcpy_s(input.pass, getPass().c_str());
		cin.clear();
		cin.ignore(10000, '\n');

		for (account &user : accounts) {
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
	}
}

namespace {
	unsigned short getId() {
		// Проверка существования
		cout << "Enter id: ";
		unsigned short id;
		while (true) {
			id = getNumber();
			if (id > 0 && id <= accounts.size())
				return id;
			else
				cout << "Invalid id: ";
		}
	}
}

void setLogin(account* a) {

}

void setPass(account* a) {

}

void setRole(account* a) {

}

int deleteAccount(unsigned short id) {
	return -1;
}

int addAccount() {
	account a;

	a.id = 1;

	return a.id;
}

unsigned short viewAccount(account* a) {
	return a->id;
}

int editAccount() {
	unsigned short id = getId();
	bool correct;
	while (true) {
		drawMenu(5, LOGIN, PASS, ROLE, REMOVE, BACK);
		drawTitles(4,
			3, "id", 25, "login", 25, "password", 24, "role");
		viewAccount(&accounts[id - 1]);
		cout << endl;
		do {
			correct = true;
			switch (_getwch()) {
			// Логин
			case '1': setLogin(&accounts[id - 1]);
				break;
			// Пароль
			case '2': setPass(&accounts[id - 1]);
			break;
			// Уровень доступа
			case '3': setRole(&accounts[id - 1]);
				break;
			// Удалить
			case '4': return deleteAccount(id);
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: correct = false;
			}
		} while (!correct);
	}
}

int viewAccounts() {
	drawTitles(4,
		3, "id", 25, "login", 25, "password", 24, "role");
	for (account &a : accounts) {
		viewAccount(&a);
	}
	return accounts.size();
}

int saveAccounts() {
	// Запись из вектора в файл
	ofstream fout(STUDLIST, ios::binary | ios::out | ios_base::trunc);
	for (account &user : accounts)
		fout.write((char*)&user, sizeof(account));
	fout.close();

	system("cls");
	cout << "All changes have been saved\n" << endl;
	cout << "press any key...";
	_getwch();

	return accounts.size();
}
