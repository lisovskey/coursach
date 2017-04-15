/*
* RitZEED inc.
*/

#include "stdafx"
#include "constants"
#include "console"
#include "stringer"
#include "presser"
#include "input"
#include "drawer"
#include "account"
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

	size_t readAccounts()
	/// Считывание аккаунтов из файла в вектор
	{
		ifstream accin(ACCLIST, ios::binary | ios::in);
		if (accin.is_open()) {
			size_t i = 1;
			account tmp;
			while (accin.peek() != EOF) {
				accin.read((char*)&tmp, sizeof(account));
				tmp.id = i++;
				accounts.push_back(tmp);
			}
			accin.close();
		}
		return accounts.size();
	}

	size_t getId()
	/// Ввод номера с проверкой существования
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

	void setLogin(account* a, const size_t y)
	/// Ввод логина только из букв и цифр
	{
		drawPreCentered(ENTER_LOGIN, y);
		char login[STRING_LENGTH + 1];
		while (true) {
			cin.getline(login, STRING_LENGTH + 1);
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
			if (strlen(login) == STRING_LENGTH) {
				cin.ignore(10000, '\n');
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(LONG_LOGIN, y);
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
				strcpy_s(a->login, login);
				return;
			}
		}
	}

	void setPass(account* a, const size_t y)
	/// Ввод пароля только из букв, цифр, дефисов и подчеркиваний
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
			else if (!regex_match(pass, regex("[0-9_A-Z-a-z]+"))) {
				TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(INVALID_PASS, y);
			}
			else {
				strcpy_s(a->pass, pass);
				return;
			}
		}
	}

	void setRole(account* a, const size_t y)
	/// Ввод роли пользователя
	{
		drawPreCentered(ENTER_ROLE, y);
		a->role = getBoolean();
	}

	size_t deleteAccount(const size_t id)
	/// Удаление аккаунта из вектора
	{
		clearScreen();
		accounts.erase(accounts.begin() + id - 1);

		// Исправление номеров
		for (size_t i = id - 1; i < accounts.size(); i++) {
			accounts[i].id = i + 1;
		}

		drawCentered(ACCOUNT_REMOVED, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return accounts.size();
	}

	size_t viewAccount(account* a)
	/// Отображение основной информации об аккаунте
	{
		string role;
		role = a->role ? "admin" : "user";

		// Отображение
		cout << " " << right << setfill('0') << setw(2)
			<< a->id << setfill(' ') << " " << left
			<< (char)179 << setw(25) << a->login
			<< (char)179 << setw(24) << a->pass
			<< (char)179 << setw(24) << role << endl;

		return a->id;
	}

}

bool auth()
/// Инициализация окна и авторизация или решистрация
{
	TConsole::initWindow(WINDOW_WIDTH, WINDOW_HEIGHT, APP_NAME.c_str());
	if (readAccounts() == 0) {
		// Отображение подсказок
		drawHelp(ROLE_ADMIN);
		waitAnyKey();
		clearScreen();

		drawCentered(REGISTRATION, 1);
		// Создание аккаунта администратора
		account admin;
		admin.id = 1;
		setLogin(&admin, WINDOW_HEIGHT / 2 - 1);
		setPass(&admin, WINDOW_HEIGHT / 2);
		admin.role = ROLE_ADMIN;
		accounts.push_back(admin);

		// Отображение
		drawAccountTitles();
		viewAccount(&admin);
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

size_t createAccount()
/// Создание аккаунта вручную
{
	clearScreen();
	account a;
	a.id = accounts.size() + 1;

	if (a.id > 99) {
		drawCentered(TOO_MUCH_ACCOUNTS, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}

	// Заполнение полей
	drawCentered(ACC_CREATING, 1);
	setLogin(&a, WINDOW_HEIGHT / 2 - 2);
	cout << endl;
	setPass(&a, WINDOW_HEIGHT / 2);
	cout << endl;
	setRole(&a, WINDOW_HEIGHT / 2 + 2);

	// Добавление в вектор
	accounts.push_back(a);

	// Отображение
	drawAccountTitles();
	viewAccount(&a);
	drawCentered(ACCOUNT_ADDED, WINDOW_HEIGHT / 2);
	waitAnyKey();
	return a.id;
}

size_t editAccount()
/// Изменение полей аккаунта
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
		viewAccount(&accounts[id - 1]);
		drawMenu(5, LOGIN, PASS, ROLE, REMOVE, BACK);
		do {
			g_correct_press = true;
			switch (getPress()) {
			// Логин
			case '1': TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setLogin(&accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Пароль
			case '2': TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setPass(&accounts[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Уровень доступа
			case '3': TConsole::clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setRole(&accounts[id - 1], WINDOW_HEIGHT / 2);
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

size_t viewAccounts()
/// Отображение всех аккаунтов
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
		viewAccount(&a);
	}
	cout << "\n" << endl;
	waitAnyKey();

	return accounts.size();
}

void writeAccounts()
/// Запись из вектора в файл
{
	ofstream accout(ACCLIST, ios::binary | ios::out | ios_base::trunc);
	for (account &user : accounts)
		accout.write((char*)&user, sizeof(account));
	accout.close();
}
