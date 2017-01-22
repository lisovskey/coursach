/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "constants"
#include "input"
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

typedef struct {
	unsigned short	id;
	char			name[32];
	unsigned int	group;
	marks			knowledge;
	double			gpa;
	credits			pass;
	circumstances	privileges;
	double			cash;
} student;

vector<student> students;

int getStudents() {
	ifstream fin(DATA, ios::binary | ios::in);
	if (fin.is_open()) {
		while (!fin.eof()) {
			student tmp;
			fin.read((char*)&tmp, sizeof(student));
			students.push_back(tmp);
		}
		fin.close();
		return students.size();
	}
	else {
		return 0;
	}
}

int createStudent() {
	return 0;
}

int generateStudent() {
	return 0;
}

unsigned short getId() {
	system("cls");
	cout << "Enter id: ";
	return (unsigned short)getNumber();
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

void findStudent() {
	unsigned short id = getId();
}

int editStudent() {
	unsigned short id = getId();
	bool correct;
	while (true) {
		drawMenu(6, NAME, GROUP, MARKS, CREDITS, CIRCS, BACK);
		do {
			correct = true;
			switch (_getwch()) {
			// Имя
			case '1': break;
			// Группу
			case '2': break;
			// Отметки
			case '3': break;
			// Зачеты
			case '4': break;
			// Льготы
			case '5': break;
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: correct = false;
			}
		} while (!correct);
	}
}

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

int saveChanges() {
	ofstream fout(DATA, ios::binary | ios::out);
	for (auto &person : students)
		fout.write((char*)&person, sizeof(student));
	fout.close();

	system("cls");
	cout << "All changes have been saved\n" << endl;
	cout << "press any key...";
	_getwch();

	return students.size();
}
