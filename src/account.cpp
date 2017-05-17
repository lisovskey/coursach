/*
* RitZEED inc.
*/

#include "stdafx.hpp"
#include "constants.hpp"
#include "console.hpp"
#include "stringer.hpp"
#include "presser.hpp"
#include "input.hpp"
#include "drawer.hpp"
#include "account.hpp"
#include <fstream>
#include <vector>
#include <regex>
using namespace std;

#define drawAccountTitles() drawTitles(4, \
	4, ID_TITLE, 25, LOGIN_TITLE, 24, PASS_TITLE, 24, ROLE_TITLE);

namespace {

	typedef struct {
		size_t		id;
		char		login[STRING_LENGTH];
		char		pass[STRING_LENGTH];
		bool		role;
	} account;

	// Вектор аккаунтов
	vector<account> accounts;

	// Считывание аккаунтов из файла в вектор
	bool readAccounts()
	{
		bool admin_exist = false;
		ifstream accin(ACCLIST, ios::binary | ios::in);
		if (accin.is_open()) {
			size_t i = 1;
			account tmp;
			while (accin.peek() != EOF) {
				accin.read((char*)&tmp, sizeof(account));
				tmp.id = i++;
				accounts.push_back(tmp);
				if (tmp.role) admin_exist = true;
			}
		}
		return admin_exist;
	}

	// Ввод номера с проверкой существования
	size_t getId()
	{
		drawPreCentered(ENTER_ID, WINDOW_HEIGHT / 2);
		size_t id;
		while (true) {
			id = getPositiveNumber();
			if (id < 1 || id > accounts.size()) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEIGHT / 2);
				drawPreCentered(INVALID_ID, WINDOW_HEIGHT / 2);
			}
			else return id;
		}
	}

	// Ввод логина только из букв и цифр
	void setLogin(account &a, const size_t y)
	{
		drawPreCentered(ENTER_LOGIN, y);
		char login[STRING_LENGTH + 1];
		while (true) {
			cin.getline(login, STRING_LENGTH + 1);
			cin.clear();
			bool already_taken = false;
			for (account &user : accounts) {
				if (strcmp(user.login, login) == 0) {
					if (user.id != a.id) {
						already_taken = true;
						break;
					}
				}
			}
			if (strlen(login) == STRING_LENGTH) {
				cin.ignore(10000, '\n');
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(LONG_LOGIN, y);
			}
			else if (strlen(login) < LOGIN_MIN_LENGTH) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(SHORT_LOGIN, y);
			}
			else if (!regex_match(login, regex("[0-9A-Za-z]+"))) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(INVALID_LOGIN, y);
			}
			else if (already_taken) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(TAKEN_LOGIN, y);
			}
			else {
				strcpy_s(a.login, login);
				return;
			}
		}
	}

	// Ввод пароля только из букв, цифр, дефисов и подчеркиваний
	void setPass(account &a, const size_t y)
	{
		drawPreCentered(ENTER_PASS, y);
		static char pass[STRING_LENGTH + 1];
		while (true) {
			cin.getline(pass, STRING_LENGTH + 1);
			cin.clear();
			if (strlen(pass) == STRING_LENGTH) {
				cin.ignore(10000, '\n');
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(LONG_PASS, y);
			}
			else if (strlen(pass) < PASS_MIN_LENGTH) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(SHORT_PASS, y);
			}
			else if (!regex_match(pass, regex("[0-9_A-Z-a-z]+"))) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(INVALID_PASS, y);
			}
			else {
				strcpy_s(a.pass, pass);
				return;
			}
		}
	}

	// Ввод роли пользователя
	void setRole(account &a, const size_t y)
	{
		drawPreCentered(ENTER_ROLE, y);
		a.role = getBoolean();
	}

	// Удаление аккаунта из вектора
	size_t deleteAccount(const size_t id)
	{
		clearScreen();
		accounts.erase(accounts.begin() + id - 1);

		// Исправление номеров
		for (size_t i = id - 1; i < accounts.size(); ++i) {
			accounts[i].id = i + 1;
		}

		drawCentered(ACCOUNT_REMOVED, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return accounts.size();
	}

	// Отображение основной информации об аккаунте
	void viewAccount(account &a)
	{
		cout << " " << right << setfill('0') << setw(2)
			<< a.id << setfill(' ') << " " << left
			<< (char)179 << setw(25) << a.login
			<< (char)179 << setw(24) << a.pass
			<< (char)179 << setw(24) << (a.role ? "admin" : "user") << endl;
	}

}

// Инициализация окна и авторизация или регистрация
bool auth()
{
	TConsole::initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME.c_str());
	if (!readAccounts()) {
		// Отображение подсказок
		drawHelp(ROLE_ADMIN);
		waitAnyKey();
		clearScreen();

		// Создание аккаунта администратора
		drawCentered(REGISTRATION, 1);
		account admin;
		admin.id = 1;
		setLogin(admin, WINDOW_HEIGHT / 2 - 1);
		setPass(admin, WINDOW_HEIGHT / 2);
		admin.role = ROLE_ADMIN;
		accounts.push_back(admin);

		// Отображение
		drawAccountTitles();
		viewAccount(admin);
		drawCentered(ADMIN_CREATED, WINDOW_HEIGHT / 2);
		waitAnyKey();
	}

	// Авторизация
	account input;
	while (true) {
		clearScreen();
		drawCentered(AUTHORIZATION, 1);
		// Ввод данных
		drawPreCentered(AUTH_LOGIN, WINDOW_HEIGHT / 2 - 1);
		if (!cin.getline(input.login, STRING_LENGTH)) {
			cin.clear();
			cin.ignore(10000, '\n');
		}
		drawPreCentered(AUTH_PASS, WINDOW_HEIGHT / 2);
		strcpy_s(input.pass, getPass(STRING_LENGTH).c_str());
		clearScreen();

		// Проверка на совпадение с каждым аккаунтом
		for (account &account : accounts) {
			if (strcmp(input.login, account.login) == 0 &&
				strcmp(input.pass, account.pass) == 0) {
				string greeting = account.login;
				greeting = "hello, " + greeting;
				drawCentered(greeting, WINDOW_HEIGHT / 2);
				waitAnyKey();

				return account.role;
			}
		}
		drawCentered(INCORRECT_AUTH, WINDOW_HEIGHT / 2);
		waitAnyKey();
	}
}

// Создание аккаунта вручную
size_t createAccount()
{
	clearScreen();
	account a;
	a.id = accounts.size() + 1;

	// Переполненный вектор
	if (a.id > 35) {
		drawCentered(TOO_MUCH_ACCOUNTS, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}

	// Заполнение полей
	drawCentered(ACC_CREATING, 1);
	setLogin(a, WINDOW_HEIGHT / 2 - 2);
	cout << endl;
	setPass(a, WINDOW_HEIGHT / 2);
	cout << endl;
	setRole(a, WINDOW_HEIGHT / 2 + 2);

	// Добавление в вектор
	accounts.push_back(a);

	// Отображение
	drawAccountTitles();
	viewAccount(a);
	drawCentered(ACCOUNT_ADDED, WINDOW_HEIGHT / 2);
	waitAnyKey();
	return a.id;
}

// Изменение полей аккаунта
size_t editAccount()
{
	clearScreen();
	// Пустой вектор
	if (accounts.size() == 0) {
		drawCentered(NOTHING_TO_EDIT, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	// Запрос номера
	size_t id = getId();
	while (true) {
		// Изменяемый аккаунт
		drawAccountTitles();
		viewAccount(accounts[id - 1]);
		drawMenu(5, LOGIN, PASS, ROLE, REMOVE, BACK);
		do {
			g_correct_press = true;
			switch (getPress()) {
			// Логин
			case '1': TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setLogin(accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Пароль
			case '2': TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setPass(accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Уровень доступа
			case '3': TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setRole(accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Удалить
			case '4': return deleteAccount(id);
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: g_correct_press = false;
			}
		} while (!g_correct_press);
	}
}

// Отображение всех аккаунтов
size_t viewAccounts()
{
	// Пустой вектор
	if (accounts.size() == 0) {
		clearScreen();
		drawCentered(NOTHING_TO_SHOW, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	// Заголовки
	drawAccountTitles();
	// Основная информация о каждом аккаунте
	for (account &a : accounts) {
		viewAccount(a);
	}
	cout << "\n" << endl;
	waitAnyKey();

	return accounts.size();
}

// Запись из вектора в файл
void writeAccounts()
{
	ofstream accout(ACCLIST, ios::binary | ios::out | ios_base::trunc);
	for (account &user : accounts)
		accout.write((char*)&user, sizeof(account));
}
