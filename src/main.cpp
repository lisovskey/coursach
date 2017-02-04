/*
* RitZEED inc.
*/

#include "presser"
#include "constants"
#include "account"
#include "student"
#include "drawer"

int userMode();
int adminMode();
int administration();
bool quit();

int main() {
	return auth() ? adminMode() : userMode();
}

int userMode() {
	getStudents();
	bool correct;
	// Основной цикл
	while (true) {
		drawMenu(4, FIND, VIEW, SORT, QUIT);
		// Цикл выбора действия
		do {
			correct = true;
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
			// Выйти из программы
			case '0': if (quit()) return 0;
				break;
			// Неверный ввод
			default: correct = false;
			} 
		} while (!correct);
	}
}

int adminMode() {
	getStudents();
	bool correct;
	// Основной цикл
	while (true) {
		drawMenu(7, ADD, EDIT, VIEW, SORT, SAVE, ACCS, QUIT);
		// Цикл выбора действия
		do {
			correct = true;
			switch (getPress()) {
			// Добавить студента
			case '1': addStudent();
				break;
			// Изменить информацию
			case '2': editStudent();
				break;
			// Сортировать список
			case '3': viewStudents();
				break;
			// Сортировать список
			case '4': sortStudents();
				break;
			// Сохранить изменения
			case '5': saveStudents();
				break;
			// Администрирование
			case '6': administration();
				break;
			// Выйти из программы
			case '0': if (quit()) return 0;
				break;
			// Неверный ввод
			default: correct = false;
			}
		} while (!correct);
	}
}

int administration() {
	bool correct;
	// Основной цикл
	while (true) {
		drawMenu(5, ADD, EDIT, VIEW, SAVE, BACK);
		// Цикл выбора действия
		do {
			correct = true;
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
			case '4': saveAccounts();
				break;
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: correct = false;
			}
		} while (!correct);
	}
}

bool quit() {
	drawMenu(2, QUIT, BACK);
	do switch (getPress()) {
	// Действительно выйти
	case '1': return true;
	// Вернуться в меню
	case '0': return false;
	} while (true);
}
