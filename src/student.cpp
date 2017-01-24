/*
* RitZEED inc.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include "constants"
#include "olives"
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
	bool			dormitory	:1;
	bool			foreign		:1;
} circumstances;

typedef struct {
	unsigned short	id;
	char			name[32];
	unsigned int	group;
	marks			knowledge;
	double			gpa;
	credits			passes;
	circumstances	privileges;
	double			cash;
} student;

vector<student> students;

int getStudents() {
	ifstream fin(DATA, ios::binary | ios::in);
	if (fin.is_open()) {
		while (fin.peek() != EOF) {
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

unsigned short getId() {
	cout << "Enter id: ";
	unsigned short id;
	while (true) {
		id = getNumber();
		if (id > 0 && id <= students.size())
			return id;
		else
			cout << "Invalid id: ";
	}
}

void setName(student* s) {
	cout << "Enter name: ";
	static char name[32];
	while (true) {
		cin.getline(name, 32);
		cin.clear();
		if (strlen(name) == 31) {
			cin.ignore(10000, '\n');
			cout << "Too long name: ";
		}
		else if (regex_match(name, regex("^[A-Z a-z]+$"))) {
			if (wordCount(name) == 2) {
				strcpy_s(s->name, name);
				return;
			}
			else {
				cout << "Name and surname: ";
			}
		}
		else {
			cout << "Invalid name: ";
		}
	}
}

void setGroup(student* s) {
	cout << "Enter group: ";
	unsigned int group;
	while (true) {
		group = getNumber();
		if (group > 99999 && group < 1000000) {
			s->group = group;
			return;
		}
		else {
			cout << "Invalid group: ";
		}
	}
}

unsigned short getMark() {
	unsigned int mark;
	while (true) {
		mark = getNumber();
		if (mark > 0 && mark < 11)
			return mark;
		else
			cout << "Invalid mark: ";
	}
}

void setMarks(student* s) {
	double sum = 0;
	cout << endl << "Enter math mark: ";
	sum += s->knowledge.math = getMark();
	cout << "Enter programming mark: ";
	sum += s->knowledge.prog = getMark();
	cout << "Enter physics mark: ";
	sum += s->knowledge.phys = getMark();
	cout << "Enter philosophy mark: ";
	sum += s->knowledge.phil = getMark();

	s->gpa = sum / 4;
}

void setCredits(student* s) {
	cout << endl << "Passed graphics credit: ";
	s->passes.graphics = getBoolean();
	cout << "Passed english credit: ";
	s->passes.english = getBoolean();
	cout << "Passed swimming credit: ";
	s->passes.swimming = getBoolean();
	cout << "Passed designing credit: ";
	s->passes.designing = getBoolean();
	cout << "Passed history credit: ";
	s->passes.history = getBoolean();
}

void setCircs(student* s) {
	cout << endl << "Is on budget: ";
	s->privileges.budget = getBoolean();
	cout << "Is activist: ";
	s->privileges.activism = getBoolean();
	cout << "Is in dormitory: ";
	s->privileges.dormitory = getBoolean();
	cout << "Is foreign: ";
	s->privileges.foreign = getBoolean();
}

void calculateCash(student* s) {
	s->cash = 0;
	// TODO
}

int createStudent() {
	system("cls");
	student s;

	setName(&s);
	setGroup(&s);
	setMarks(&s);
	setCredits(&s);
	setCircs(&s);

	calculateCash(&s);
	s.id = students.size() + 1;
	students.push_back(s);
	
	cout << "\nStudent has been added\n" << endl;
	cout << "press any key...";
	_getwch();
	return s.id;
}

int generateStudent() {
	// TODO
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

int findStudent() {
	system("cls");
	if (students.size() == 0) {
		cout << "There is nothing to look for\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	char request[32];
	cin.getline(request, 32);
	cin.clear();
	if (strlen(request) == 31)
		cin.ignore(10000, '\n');

	for (auto person : students) {
		if (strstr(person.name, request) != NULL) {
			// TODO
			cout << "press any key...";
			_getwch();
			return person.id;
		}
	}
	return 0;
}

int deleteStudent(int id) {
	system("cls");
	students.erase(students.begin() + id - 1);
	cout << "Student was deleted\n" << endl;
	cout << "press any key...";
	_getwch();
	return -1;
}

int editStudent() {
	system("cls");
	if (students.size() == 0) {
		cout << "There is nothing to edit\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	unsigned short id = getId();
	bool correct;
	while (true) {
		drawMenu(7, NAME, GROUP, MARKS, CREDITS, CIRCS, REMOVE, BACK);
		do {
			correct = true;
			switch (_getwch()) {
			// Имя
			case '1': setName(&students[id - 1]);
				break;
			// Группу
			case '2': setGroup(&students[id - 1]);
				break;
			// Отметки
			case '3': setMarks(&students[id - 1]);
				break;
			// Зачеты
			case '4': setCredits(&students[id - 1]);
				break;
			// Льготы
			case '5': setCircs(&students[id - 1]);
				break;
			// Удалить
			case '6': return deleteStudent(id);
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: correct = false;
			}
		} while (!correct);
	}
}

int viewStudents() {
	if (students.size() == 0) {
		cout << "There is nothing to show\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
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
	if (students.size() == 0) {
		cout << "There is nothing to save\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	ofstream fout(DATA, ios::binary | ios::out | ios_base::trunc);
	for (auto person : students)
		fout.write((char*)&person, sizeof(student));
	fout.close();

	system("cls");
	cout << "All changes have been saved\n" << endl;
	cout << "press any key...";
	_getwch();

	return students.size();
}
