/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <regex>
#include <windows.h>
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
	char			name[24];
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
	static char name[24];
	while (true) {
		cin.getline(name, 24);
		cin.clear();
		if (strlen(name) == 23) {
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
	cout << "Enter math mark: ";
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
	cout << "Passed graphics credit: ";
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
	cout << "Is on budget: ";
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
	cout << endl;
	setMarks(&s);
	cout << endl;
	setCredits(&s);
	cout << endl;
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

void viewList(vector<student> list, int id = NULL) {
	int i, to;
	bool stop;
	if (id == NULL) {
		system("cls");
		i = 0;
		to = list.size();
		stop = true;
	}
	else {
		i = id - 1;
		to = id;
		stop = false;
	}

	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, 15);
	cout << left << setw(2) << "id"
		<< (char)179 << setw(24) << "name"
		<< (char)179 << setw(11) << "group"
		<< (char)179 << setw(9) << "gpa"
		<< (char)179 << setw(9) << "passed"
		<< (char)179 << setw(9) << "circs"
		<< (char)179 << setw(11) << "cash";
	SetConsoleTextAttribute(hConsole, 240);

	while (i < to) {
		student person = list[i];
		string passes = "";
		int count = 0;
		if (person.passes.graphics)
			count++;
		if (person.passes.english)
			count++;
		if (person.passes.swimming)
			count++;
		if (person.passes.designing)
			count++;
		if (person.passes.history)
			count++;
		passes += '0' + count;
		passes += "/5";

		string circs = "";
		if (person.privileges.budget)
			circs += "b ";
		if (person.privileges.activism)
			circs += "a ";
		if (person.privileges.dormitory)
			circs += "d ";
		if (person.privileges.foreign)
			circs += "f";
		if (circs.length() == 0)
			circs += "-";

		cout << endl << right << setfill('0') << setw(2)
			<< person.id << left << setfill(' ')
			<< (char)179 << setw(24) << person.name
			<< (char)179 << setw(11) << person.group
			<< (char)179 << setw(9) << person.gpa
			<< (char)179 << setw(9) << passes
			<< (char)179 << setw(9) << circs
			<< (char)179 << setw(11) << fixed 
			<< setprecision(2) << person.cash;

		i++;
	}
	cout << endl << endl;

	if (stop) {
		cout << "press any key...";
		_getwch();
	}
}

int findStudent() {
	system("cls");
	if (students.size() == 0) {
		cout << "There is nothing to look for\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	string request;
	cout << "search: ";
	getline(cin, request);
	cin.clear();
	for (int i = 0; i < request.length(); ++i)
		request[i] = tolower(request[i]);

	system("cls");
	string tmp;
	for (auto person : students) {
		tmp = person.name;
		for (int i = 0; i < tmp.length(); ++i)
			tmp[i] = tolower(tmp[i]);
		if (tmp.find(request) != string::npos) {
			viewList(students, person.id);
			cout << "press any key...";
			_getwch();
			return person.id;
		}
	}

	cout << "Nothong found\n" << endl;
	cout << "press any key...";
	_getwch();
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
		viewList(students, id);
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
	case '1': viewList(students);
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
