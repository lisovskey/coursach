/*
* RitZEED inc.
*/

#include <conio.h>
#include "constants"
#include "menu"
#include "student"

int auth();
int userMode();
int adminMode();
int quit();
bool correct;

int main(int argc, char* argv[]) {
	return auth() ? adminMode() : userMode();
}

int auth() {
	drawMenu(2, ADMIN, USER);
	// Цикл выбора режимар
	do switch (_getwch()) {
		correct = true;
	// Режим администратора
	case '1': return 1;
	// Режим пользователя
	case '0': return 0;
	// Неверный ввод
	default: correct = false;
	} while (!correct);
}

int userMode() {
	// Основной цикл
	while (true) {
		drawMenu(3, FIND, SORT, QUIT);
		// Цикл выбора действия
		do switch (_getwch()) {
			correct = true;
		// Найти студента
		case '1': findStudent();
			break;
		// Сортировать список
		case '2': sortStudents();
			break;
		// Выйти из программы
		case '0': return 0;
		// Неверный ввод
		default: correct = false;
		} while (!correct);
	}
}

int adminMode() {
	// Основной цикл
	while (true) {
		drawMenu(5, ADD, EDIT, SORT, SAVE, QUIT);
		// Цикл выбора действия
		do switch (_getwch()) {
			correct = true;
		// Добавить студента
		case '1': addStudent();
			break;
		// Изменить информацию
		case '2': editStudent();
			break;
		// Сортировать список
		case '3': sortStudents();
			break;
		// Сохранить изменения
		case '4': saveChanges();
			break;
		// Выйти из программы
		case '0': return quit();
		// Неверный ввод
		default: correct = false;
		} while (!correct);
	}
}

int quit() {
	drawMenu(2, SAVE_N_QUIT, QUIT);
	// Цикл выбора действия
	do switch (_getwch()) {
		correct = true;
	// Сохранить и выйти
	case '1': saveChanges();
	// Выйти без сохранения
	case '0': return 0;
	// Неверный ввод
	default: correct = false;
	} while (!correct);
}