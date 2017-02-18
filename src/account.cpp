/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
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
		return accounts.size();
	}

	size_t getId() {
		// Проверка существования
		drawPreCentered("Enter id: ", WINDOW_HEIGHT / 2);
		size_t id;
		while (true) {
			id = getPositiveNumber();
			if (id > 0 && id <= accounts.size())
				return id;
			else {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEIGHT / 2);
				drawPreCentered("Invalid id: ", WINDOW_HEIGHT / 2);
			}
		}
	}

	void setLogin(account* a, size_t y) {
		// Логин только из букв и цифр
		drawPreCentered("Enter login: ", y);
		static char login[21];
		while (true) {
			cin.getline(login, 21);
			cin.clear();
			bool already_taken = false;
			for (account &user : accounts) {
				if (strcmp(user.login, login) == 0) {
					if (user.id != a->id) {
						already_taken = true;
						break;
					}
				}
			}
			if (strlen(login) == 20) {
				cin.ignore(10000, '\n');
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Too long login: ", y);
			}
			else if (!regex_match(login, regex("[0-9A-Za-z]+"))) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Only digits and letters: ", y);
			}
			else if (already_taken) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Already taken: ", y);
			}
			else {
				strcpy_s(a->login, login);
				return;
			}
		}
	}

	void setPass(account* a, size_t y) {
		// Пароль только из букв и цифр
		drawPreCentered("Enter pass: ", y);
		static char pass[21];
		while (true) {
			cin.getline(pass, 21);
			cin.clear();
			if (strlen(pass) == 20) {
				cin.ignore(10000, '\n');
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Too long pass: ", y);
			}
			else if (regex_match(pass, regex("[0-9_A-Z-a-z]+"))) {
				strcpy_s(a->pass, pass);
				return;
			}
			else {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Don't use special chars: ", y);
			}
		}
	}

	void setRole(account* a, size_t y) {
		// Администратор или пользователь
		drawPreCentered("Is admin: ", y);
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
		drawCentered("Account was deleted", WINDOW_HEIGHT / 2);
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
	initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "coursach");
	if (getAccounts() == 0) {
		drawCentered(REGISTRATION, 1);
		// Создание аккаунта администратора
		account admin;
		admin.id = 1;
		setLogin(&admin, WINDOW_HEIGHT / 2 - 1);
		setPass(&admin, WINDOW_HEIGHT / 2);
		admin.admin = true;
		accounts.push_back(admin);

		drawTitles(4,
			3, "id", 25, "login", 25, "password", 24, "role");
		viewAccount(&admin);
		drawCentered("Admin account has been created", WINDOW_HEIGHT / 2);
		waitAnyKey();
	}
	// Авторизация
	account input;
	while (true) {
		system("cls");
		drawCentered(AUTHORIZATION, 1);
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
				drawCentered(greeting.c_str(), WINDOW_HEIGHT / 2);
				waitAnyKey();

				return account.admin;
			}
		}
		drawCentered("Incorrect login or password", WINDOW_HEIGHT / 2);
		waitAnyKey();
	}
}

size_t createAccount() {
	// Создание аккаунта
	system("cls");
	account a;
	a.id = accounts.size() + 1;

	if (a.id > 99) {
		drawCentered("Too much accounts", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}

	drawCentered(ACC_CREATING, 1);
	setLogin(&a, WINDOW_HEIGHT / 2 - 2);
	cout << endl;
	setPass(&a, WINDOW_HEIGHT / 2);
	cout << endl;
	setRole(&a, WINDOW_HEIGHT / 2 + 2);

	// Добавление в вектор
	accounts.push_back(a);
	drawTitles(4,
		3, "id", 25, "login", 25, "password", 24, "role");
	viewAccount(&a);
	drawCentered("Account has been added", WINDOW_HEIGHT / 2);
	waitAnyKey();
	return a.id;
}

size_t editAccount() {
	system("cls");
	// Пустой вектор
	if (accounts.size() == 0) {
		drawCentered("There is nothing to edit", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	// Запрос номера
	size_t id = getId();
	bool correct;
	while (true) {
		drawTitles(4,
			3, "id", 25, "login", 25, "password", 24, "role");
		viewAccount(&accounts[id - 1]);
		drawMenu(5, LOGIN, PASS, ROLE, REMOVE, BACK);
		do {
			correct = true;
			switch (getPress()) {
			// Логин
			case '1': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setLogin(&accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Пароль
			case '2': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setPass(&accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Уровень доступа
			case '3': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setRole(&accounts[id - 1], WINDOW_HEIGHT / 2);
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
	// Пустой вектор
	if (accounts.size() == 0) {
		system("cls");
		drawCentered("There is nothing to show", WINDOW_HEIGHT / 2);
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

	drawCentered("All changes have been saved", WINDOW_HEIGHT / 2);
	waitAnyKey();

	return accounts.size();
}
