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
#include "console"
#include "stringer"
#include "presser"
#include "input"
#include "drawer"
#include "account"
using namespace std;

namespace {
	// Внутрефайловые имена

	typedef struct {
		size_t			id;
		char			login[20];
		char			pass[20];
		bool			admin;
	} account;

	// Вектор аккаунтов
	vector<account> accounts;

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

	size_t getAccounts() {
		// Считывание из файла в вектор
		ifstream fin(ACCLIST, ios::binary | ios::in);
		if (fin.is_open()) {
			size_t i = 1;
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
		return accounts.size();
	}

	size_t getId() {
		// Проверка существования
		cout << "Enter id: ";
		size_t id;
		while (true) {
			id = getPositiveNumber();
			if (id > 0 && id <= accounts.size())
				return id;
			else
				cout << "Invalid id: ";
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

	size_t deleteAccount(size_t id) {
		// Удаление аккаунта из вектора
		system("cls");
		accounts.erase(accounts.begin() + id - 1);
		// Исправление номеров
		for (size_t i = id - 1; i < accounts.size(); i++) {
			accounts[i].id = i + 1;
		}
		drawCentered("Account was deleted");
		waitAnyKey();
		return accounts.size();
	}

	size_t viewAccount(account* a) {
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
}

bool auth() {
	getAccounts();
	TConsole tc;
	SetConsoleTitle("coursach");
	tc.Window(WINDOW_WIDTH, WINDOW_HEIGHT);
	tc.TextColor(COLOR_WHITE);
	tc.BackgroundColor(COLOR_BLACK);
	system("cls");
	// Авторизация
	account input;
	while (true) {
		// Ввод данных
		drawPreCentered("login: ", WINDOW_HEIGHT / 2 - 1);
		if (!cin.getline(input.login, 20)) {
			cin.clear();
			cin.ignore(10000, '\n');
		}
		drawPreCentered("password: ", WINDOW_HEIGHT / 2);
		strcpy_s(input.pass, getPass(20).c_str());		
		system("cls");

		// Проверка на совпадение с каждым аккаунтом
		for (account &account : accounts) {
			if (strcmp(input.login, account.login) == 0 &&
				strcmp(input.pass, account.pass) == 0) {
				string greeting = account.login;
				greeting = "hello, " + greeting;
				drawCentered(greeting.c_str());
				waitAnyKey();

				return account.admin;
			}
		}
		drawCentered("Incorrect login or password");
		waitAnyKey();
	}
}

size_t createAccount() {
	// Создание аккаунта
	system("cls");
	account a;

	setLogin(&a);
	cout << endl;
	setPass(&a);
	cout << endl;
	setRole(&a);
	a.id = accounts.size() + 1;

	// Добавление в вектор
	accounts.push_back(a);
	system("cls");
	drawTitles(4,
		3, "id", 25, "login", 25, "password", 24, "role");
	viewAccount(&a);
	drawCentered("Account has been added");
	waitAnyKey();
	return a.id;
}

size_t editAccount() {
	system("cls");
	if (accounts.size() == 0) {
		drawCentered("There is nothing to edit");
		waitAnyKey();
		return 0;
	}
	size_t id = getId();
	bool correct;
	while (true) {
		drawMenu(5, LOGIN, PASS, ROLE, REMOVE, BACK);
		drawTitles(4,
			3, "id", 25, "login", 25, "password", 24, "role");
		viewAccount(&accounts[id - 1]);
		cout << endl;
		do {
			correct = true;
			switch (getPress()) {
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

size_t viewAccounts() {
	system("cls");
	if (accounts.size() == 0) {
		drawCentered("There is nothing to show");
		waitAnyKey();
		return 0;
	}
	// Заголовки
	drawTitles(4,
		3, "id", 25, "login", 25, "password", 24, "role");
	// Основная информация о каждом аккаунте
	for (account &a : accounts) {
		viewAccount(&a);
	}
	cout << "\n" << endl;
	waitAnyKey();

	return accounts.size();
}

size_t saveAccounts() {
	system("cls");
	// Запись из вектора в файл
	ofstream fout(ACCLIST, ios::binary | ios::out | ios_base::trunc);
	for (account &user : accounts)
		fout.write((char*)&user, sizeof(account));
	fout.close();

	drawCentered("All changes have been saved");
	waitAnyKey();

	return accounts.size();
}
