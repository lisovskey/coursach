/*
* RitZEED inc.
*/

#include <conio.h>
#include "auth"
#include "constants"
#include "menu"
#include "student"

int userMode();
int adminMode();
bool quit();

int main(void) {
	return auth() ? adminMode() : userMode();
}

int userMode() {
	getStudents();
	bool correct;
	// Основной цикл
	while (true) {
		drawMenu(3, FIND, VIEW, QUIT);
		// Цикл выбора действия
		do {
			correct = true;
			switch (_getwch()) {
			// Найти студента
			case '1': findStudent();
				break;
			// Сортировать список
			case '2': viewStudents();
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
		drawMenu(5, ADD, EDIT, VIEW, SAVE, QUIT);
		// Цикл выбора действия
		do {
			correct = true;
			switch (_getwch()) {
			// Добавить студента
			case '1': addStudent();
				break;
			// Изменить информацию
			case '2': editStudent();
				break;
			// Сортировать список
			case '3': viewStudents();
				break;
			// Сохранить изменения
			case '4': saveChanges();
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

bool quit() {
	drawMenu(2, QUIT, BACK);
	do switch (_getwch()) {
	// Действительно выйти
	case '1': return true;
	// Вернуться в меню
	case '0': return false;
	} while (true);
}