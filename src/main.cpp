/*
* RitZEED inc.
*/

#include "stdafx"
#include "presser"
#include "constants"
#include "account"
#include "student"
#include "drawer"

int userMode();
int adminMode();
int administration();
void saveChanges();
void help(bool);
bool quit();

int main() {
	return auth() ? adminMode() : userMode();
}

int userMode() {
	readData();
	// Основное меню
	while (true) {
		clearScreen();
		drawCentered(MENU, 1);
		drawMenu(5, FIND, VIEW, SORT, HELP, QUIT);
		// Цикл выбора действия
		do {
			g_correct_press = true;
			switch (getPress()) {
			// Найти студента
			case '1': findStudent();		
				break;
			// Сортировать список
			case '2': viewStudents();		
				break;
			// Сортировать список
			case '3': sortStudents();		
				break;
			// Помощь
			case '4': help(ROLE_USER);
				break;
			// Выйти из программы
			case '0': if (quit()) return 0;	
				break;
			// Неверный ввод
			default: g_correct_press = false;
			} 
		} while (!g_correct_press);
	}
}

int adminMode() {
	readData();
	// Основное меню
	while (true) {
		clearScreen();
		drawCentered(MENU, 1);
		drawMenu(10, ADD, FIND, EDIT, VIEW, SORT, SAVE, SETTINGS, ACCS, HELP, QUIT);
		// Цикл выбора действия
		do {
			g_correct_press = true;
			switch (getPress()) {
			// Добавить студента
			case '1': addStudent();			
				break;
			// Найти студента
			case '2': findStudent();		
				break;
			// Изменить информацию
			case '3': editStudent();		
				break;
			// Сортировать список
			case '4': viewStudents();		
				break;
			// Сортировать список
			case '5': sortStudents();		
				break;
			// Сохранить изменения
			case '6': saveChanges();		
				break;
			// Настройки
			case '7': settings();			
				break;
			// Администрирование
			case '8': administration();		
				break;
			// Помощь
			case '9': help(ROLE_ADMIN);
				break;
			// Выйти из программы
			case '0': if (quit()) return 0;	
				break;
			// Неверный ввод
			default: g_correct_press = false;
			}
		} while (!g_correct_press);
	}
}

int administration() {
	// Меню администрирования
	while (true) {
		clearScreen();
		drawCentered(ADMINING, 1);
		drawMenu(5, ADD, EDIT, VIEW, SAVE, BACK);
		// Цикл выбора действия
		do {
			g_correct_press = true;
			switch (getPress()) {
			// Добавить аккаунт
			case '1': createAccount();		
				break;
			// Сортировать список
			case '2': editAccount();		
				break;
			// Сортировать список
			case '3': viewAccounts();		
				break;
			// Сохранить изменения
			case '4': saveChanges();		
				break;
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: g_correct_press = false;
			}
		} while (!g_correct_press);
	}
}

void saveChanges() {
	clearScreen();
	writeAccounts();
	writeStudents();
	writeConfig();
	drawCentered(CHANGES_SAVED, WINDOW_HEIGHT / 2);
	waitAnyKey();
}

void help(bool role) {
	drawHelp(role);
	waitAnyKey();
}

bool quit() {
	clearScreen();
	drawCentered(QUIT_OR_NO, 1);
	drawMenu(2, QUIT, BACK);
	do switch (getPress()) {
	// Действительно выйти
	case '1': return true;
	// Вернуться в меню
	case '0': return false;
	} while (true);
}
