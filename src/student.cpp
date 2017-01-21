/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include "constants"
#include "menu"
#include "student"
using namespace std;

typedef struct {
	unsigned short	math		:4;
	unsigned short	prog		:4;
	unsigned short	phys		:4;
	unsigned short	phil		:4;
} marks;

typedef struct {
	bool			graphics	:1;
	bool			english		:1;
	bool			swimming	:1;
	bool			designing	:1;
	bool			history		:1;
} credits;

typedef struct {
	bool			budget		:1;
	bool			activism	:1;
	bool			poverty		:1;
	bool			foreign		:1;
} circumstances;

struct student {
	unsigned short	id;
	string			name;
	unsigned int	group;
	marks			knowledge;
	double			gpa;
	credits			pass;
	circumstances	privileges;
	double			cash;
};

int createStudent() {
	return 0;
}

int generateStudent() {
	return 0;
}

int addStudent() {
	drawMenu(3, MANUAL, GENERATE, BACK);
	do switch (_getwch()) {
	// Добавить вручную
	case '1': return createStudent();
	// Сгенерировать
	case '2': return generateStudent();
	// Вернуться
	case '0': return 0;
	} while (true);
}

void findStudent() {}

void editStudent() {}

int viewStudents() {
	drawMenu(5, BY_NO, BY_NAME, BY_CASH, BY_GPA, BACK);
	do switch (_getwch()) {
	// По номеру
	case '1': return 1;
	// По имени
	case '2': return 2;
	// По стипендии
	case '3': return 3;
	// По среднему баллу
	case '4': return 4;
	// Вернуться
	case '0': return 0;
	} while (true);
}

void saveChanges() {}
