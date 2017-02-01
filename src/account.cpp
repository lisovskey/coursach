/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <regex>
#include <windows.h>
#include "constants"
#include "olives"
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

// Вектор аккаунтов
vector<account> accounts;

string getPass() {
	// Скрытие ввода пароля
	string result;

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
	while (ReadConsoleA(ih, &c, 1, &count, NULL) && (c != '\r') && (c != '\n')) {
		if (c == '\b') {
			if (result.length()) {
				WriteConsoleA(oh, "\b \b", 3, &count, NULL);
				result.erase(result.end() - 1);
			}
		}
		else {
			if (result.length() < 19) {
				WriteConsoleA(oh, symbol, 1, &count, NULL);
				result.push_back(c);
			}
		}
	}
	SetConsoleMode(ih, mode);

	return result;
}

unsigned getAccounts() {
	// Считывание из файла в вектор
	ifstream fin(ACCLIST, ios::binary | ios::in);
	if (fin.is_open()) {
		unsigned i = 1;
		account tmp;
		while (fin.peek() != EOF) {
			fin.read((char*)&tmp, sizeof(account));
			tmp.id = i++;
			accounts.push_back(tmp);
		}
		fin.close();
	}
	else {
		// Создание аккаунта администратора
		account admin;
		admin.id = 1;
		strcpy_s(admin.login, "admin");
		strcpy_s(admin.pass, "admin");
		admin.admin = true;
		accounts.push_back(admin);
	}
	// Установка основы рандома
	srand((unsigned int)time(NULL));

	return accounts.size();
}

bool auth() {
	getAccounts();
	// Авторизация
	account input;
	while (true) {
		// Ввод данных
		system("cls");
		cout << "login: ";
		cin >> input.login;
		cout << "pass: ";
		strcpy_s(input.pass, getPass().c_str());
		cin.clear();
		cin.ignore(10000, '\n');

		// Проверка на совпадение с каждым аккаунтом
		for (account &user : accounts) {
			if (strcmp(input.login, user.login) == 0 &&
				strcmp(input.pass, user.pass) == 0) {
				system("cls");
				cout << "hello, " << user.login << endl;
				waitAnyKey();

				return user.admin;
			}
		}
		system("cls");
		cout << "Incorrect login or password" << endl;
		waitAnyKey();
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
	// Логин только из букв и цифр
	cout << "Enter login: ";
	static char login[21];
	while (true) {
		cin.getline(login, 21);
		cin.clear();
		if (strlen(login) == 20) {
			cin.ignore(10000, '\n');
			cout << "Too long login: ";
		}
		else if (regex_match(login, regex("[0-9A-Za-z]+"))) {
			strcpy_s(a->login, login);
			return;
		}
		else {
			cout << "Only digits and letters: ";
		}
	}
}

void setPass(account* a) {
	// Пароль только из букв и цифр
	cout << "Enter pass: ";
	static char pass[21];
	while (true) {
		cin.getline(pass, 21);
		cin.clear();
		if (strlen(pass) == 20) {
			cin.ignore(10000, '\n');
			cout << "Too long pass: ";
		}
		else if (regex_match(pass, regex("[0-9_A-Z-a-z]+"))) {
			strcpy_s(a->pass, pass);
			return;
		}
		else {
			cout << "Don't use special chars: ";
		}
	}
}

void setRole(account* a) {
	// Администратор или пользователь
	cout << "Is admin: ";
	a->admin = getBoolean();
}

int deleteAccount(unsigned short id) {
	// Удаление аккаунта из вектора
	system("cls");
	accounts.erase(accounts.begin() + id - 1);
	// Исправление номеров
	for (unsigned i = id - 1; i < accounts.size(); i++) {
		accounts[i].id = i + 1;
	}
	cout << "Account was deleted" << endl;
	waitAnyKey();
	return -1;
}

unsigned short createAccount() {
	// Создание аккаунта
	system("cls");
	account a;

	setLogin(&a);
	cout << endl;
	setPass(&a);
	cout << endl;
	setRole(&a);
	a.id = (unsigned short)accounts.size() + 1;

	// Добавление в вектор
	accounts.push_back(a);
	cout << "\nAccount has been added" << endl;
	waitAnyKey();
	return a.id;
}

unsigned short viewAccount(account* a) {
	// Запись прав
	string role;
	if (a->admin) 
		role = "admin";
	else 
		role = "user";
	
	// Отображение
	cout << right << setfill('0') << setw(2)
		<< a->id << setfill(' ') << " " << left
		<< (char)179 << setw(25) << a->login
		<< (char)179 << setw(25) << a->pass
		<< (char)179 << setw(24) << role << endl;

	return a->id;
}

int editAccount() {
	system("cls");
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

unsigned viewAccounts() {
	system("cls");
	// Заголовки
	drawTitles(4,
		3, "id", 25, "login", 25, "password", 24, "role");
	// Основная информация о каждом аккаунте
	for (account &a : accounts) {
		viewAccount(&a);
	}
	cout << endl;
	waitAnyKey();

	return accounts.size();
}

unsigned saveAccounts() {
	system("cls");
	// Запись из вектора в файл
	ofstream fout(ACCLIST, ios::binary | ios::out | ios_base::trunc);
	for (account &user : accounts)
		fout.write((char*)&user, sizeof(account));
	fout.close();

	cout << "All changes have been saved" << endl;
	waitAnyKey();

	return accounts.size();
}
