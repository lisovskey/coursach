﻿/*
* RitZEED inc.
*/

#include <conio.h>
#include "auth"
#include "constants"
#include "menu"
#include "student"

int userMode();
int adminMode();
int quit();

int main(void) {
	return auth() ? adminMode() : userMode();
}

int userMode() {
	bool correct;
	// Основной цикл
	while (true) {
		drawMenu(3, FIND, SORT, QUIT);
		// Цикл выбора действия
		do {
			correct = true;
			switch (_getwch()) {
			// Найти студента
			case '1': findStudent();
				break;
			// Сортировать список
			case '2': sortStudents();
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
	bool correct;
	// Основной цикл
	while (true) {
		drawMenu(5, ADD, EDIT, SORT, SAVE, QUIT);
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
			case '3': sortStudents();
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

int quit() {
	drawMenu(2, QUIT, BACK);
	// Цикл выбора действия
	while (true) {
		switch (_getwch()) {
		// Действительно выйти
		case '1': return 1;
		// Вернуться в меню
		case '0': return 0;
		}
	}
}