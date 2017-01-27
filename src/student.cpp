/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <windows.h>
#include "constants"
#include "olives"
#include "input"
#include "menu"
#include "generator"
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
	bool			science		:1;
	bool			foreign		:1;
	bool			invalid		:1;
	bool			dormitory	:1;
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

bool calculateCash(student* s) {
	// Расчет стипендии
	double cash;
	// Исключение
	if (s->knowledge.math < 4 ||
		s->knowledge.prog < 4 ||
		s->knowledge.phys < 4 ||
		s->knowledge.phil < 4) {
		return false;
	}
	// Базовая стипендия
	if (s->privileges.foreign) {
		cash = 167.74;
	}
	else if (s->privileges.budget) {
		if (!(s->passes.graphics &&
			s->passes.english &&
			s->passes.swimming &&
			s->passes.designing &&
			s->passes.history)) {
			cash = 0;
		}
		else cash = 58.28;
	}
	else {
		if (s->privileges.invalid)
			cash = 58.28 * 0.8;
		else cash = 0;
	}
	// Множители
	if (cash > 1) {
		if (s->gpa < 5)
			cash *= 0.8;
		else if (s->gpa > 5.99 && s->gpa < 8)
			cash *= 1.2;
		else if (s->gpa > 7.99 && s->gpa < 9)
			cash *= 1.4;
		else if (s->gpa > 8.99)
			cash *= 1.6;

		if (s->privileges.invalid)
			cash *= 1.5;
		if (s->privileges.activism)
			cash *= 1.1;
		if (s->privileges.science)
			cash *= 1.2;
		if (s->privileges.dormitory)
			cash -= 16.42;
	}
	// Не вылетает
	s->cash = cash;
	return true;
}

int getStudents() {
	// Установка основы рандома
	srand((unsigned int)time(NULL));
	// Считывание из файла в вектор
	ifstream fin(DATA, ios::binary | ios::in);
	if (fin.is_open()) {
		int i = 0;
		while (fin.peek() != EOF) {
			student tmp;
			fin.read((char*)&tmp, sizeof(student));
			calculateCash(&tmp);
			tmp.id = ++i;
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
	// Проверка существования
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
	// Имя из двух слов только из букв
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
	// Шестизначный номер группы
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
	// Валидная отметка
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
	// Установка отметок с расчетом среднего балла
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
	// Установка вовремя сданных зачетов
	cout << "Passed graphics on time: ";
	s->passes.graphics = getBoolean();
	cout << "Passed english on time: ";
	s->passes.english = getBoolean();
	cout << "Passed swimming on time: ";
	s->passes.swimming = getBoolean();
	cout << "Passed designing on time: ";
	s->passes.designing = getBoolean();
	cout << "Passed history on time: ";
	s->passes.history = getBoolean();
}

void setCircs(student* s) {
	// Установка влияющих факторов
	cout << "Is on budget: ";
	s->privileges.budget = getBoolean();
	cout << "Is activist: ";
	s->privileges.activism = getBoolean();
	cout << "Do scince: ";
	s->privileges.science = getBoolean();
	cout << "Is foreign: ";
	s->privileges.foreign = getBoolean();
	cout << "Is invalid: ";
	s->privileges.invalid = getBoolean();
	cout << "Lives in dormitory: ";
	s->privileges.dormitory = getBoolean();
}

int createStudent() {
	// Создание студента
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

	s.id = (unsigned short)students.size() + 1;

	// В случае низких отметок
	if (!calculateCash(&s)) {
		cout << "\nStudent cannot study here\n" << endl;
		cout << "press any key...";
		_getwch();
		return students.size();
	}

	// Добавление в вектор
	students.push_back(s);
	cout << "\nStudent has been added\n" << endl;
	cout << "press any key...";
	_getwch();
	return s.id;
}

int generateStudent() {
	// Генерация студента
	system("cls");
	student s;

	strcpy_s(s.name, generateName().c_str());
	s.group = generateGroup();

	// Отметки
	s.gpa = 0;
	s.gpa += s.knowledge.math = generateMark();
	s.gpa += s.knowledge.prog = generateMark();
	s.gpa += s.knowledge.phys = generateMark();
	s.gpa += s.knowledge.phil = generateMark();
	s.gpa /= 4;

	// Другие факторы
	s.privileges.budget = generateBool((int)s.gpa / 3);
	s.privileges.invalid = generateBool(-8);
	s.privileges.activism = generateBool(-3 + s.privileges.budget);
	s.privileges.science = generateBool((int)(s.gpa + s.privileges.invalid) / 4);
	s.privileges.foreign = generateBool(-4 - 2 * s.privileges.invalid);
	s.privileges.dormitory = generateBool(2 + s.privileges.invalid + s.privileges.activism);
	
	// Своевременные зачеты
	s.passes.graphics = generateBool((int)s.gpa + 2 * s.privileges.budget);
	s.passes.designing = generateBool((int)s.gpa + 2 * s.privileges.budget);
	s.passes.english = generateBool((int)s.gpa + 2 * s.privileges.budget);
	s.passes.swimming = generateBool((int)s.gpa + 2 * s.privileges.budget);
	s.passes.history = generateBool((int)s.gpa + 2 * s.privileges.budget);

	s.id = (unsigned short)students.size() + 1;
	calculateCash(&s);

	// Добавление в вектор
	students.push_back(s);
	cout << "Student has been added\n" << endl;
	cout << "press any key...";
	_getwch();
	return s.id;
}

int deleteStudent(int id) {
	// Удаление студента из вектора
	system("cls");
	students.erase(students.begin() + id - 1);
	// Исправление номеров
	for (unsigned short i = id - 1; i < students.size(); i++) {
		students[i].id = i + 1;
	}
	cout << "Student was deleted\n" << endl;
	cout << "press any key...";
	_getwch();
	return -1;
}

void viewTitles() {
	// Смена цвета консоли
	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, 15);
	// Заголовки таблицы
	cout << left << setw(3) << "id"
		<< (char)179 << setw(24) << "name"
		<< (char)179 << setw(10) << "group"
		<< (char)179 << setw(8) << "gpa"
		<< (char)179 << setw(8) << "passed"
		<< (char)179 << setw(11) << "circs"
		<< (char)179 << setw(11) << "cash";
	SetConsoleTextAttribute(hConsole, 240);
	cout << endl;
}

int viewStudent(student* s) {
	// Расчет зачетов
	string passes = "";
	int count = 0;
	if (s->passes.graphics)
		count++;
	if (s->passes.english)
		count++;
	if (s->passes.swimming)
		count++;
	if (s->passes.designing)
		count++;
	if (s->passes.history)
		count++;
	passes += '0' + count;
	passes += "/5";

	// Другие факторы
	string circs = "";
	if (s->privileges.budget)
		circs += "b ";
	if (s->privileges.activism)
		circs += "a ";
	if (s->privileges.science)
		circs += "s ";
	if (s->privileges.foreign)
		circs += "f ";
	if (s->privileges.foreign)
		circs += "i ";
	if (s->privileges.dormitory)
		circs += "d";
	if (circs.length() == 0)
		circs += "-";

	// Отображение
	cout << right << setfill('0') << setw(2)
		<< s->id << setfill(' ') << " " << left
		<< (char)179 << setw(24) << s->name
		<< (char)179 << setw(10) << s->group
		<< (char)179 << setw(8) << s->gpa
		<< (char)179 << setw(8) << passes
		<< (char)179 << setw(11) << circs
		<< (char)179 << setw(11) << fixed
		<< setprecision(2) << s->cash << endl;

	return s->id;
}

int viewList(vector<student> &list) {
	system("cls");
	// Заголовки
	viewTitles();
	// Основная информация о каждом студенте
	for (student &s : list) {
		viewStudent(&s);
	}
	cout << endl;
	
	cout << "press any key...";
	_getwch();
	return 0;
}

int viewSortedList(vector<student> list, bool (*compare)(student, student)) {
	// Сортировка по переданной функции
	sort(list.begin(), list.end(), compare);
	return viewList(list);
}

bool byName(student a, student b) {
	return strcmp(a.name, b.name) < 0;
}

bool byCash(student a, student b) {
	return a.cash > b.cash;
}

bool byGPA(student a, student b) {
	return a.gpa > b.gpa;
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
	// Пустой вектор
	if (students.size() == 0) {
		cout << "There is nothing to look for\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	// Ввод запроса
	string request;
	cout << "search: ";
	getline(cin, request);
	cin.clear();
	for (unsigned short i = 0; i < request.length(); i++)
		request[i] = tolower(request[i]);

	// Сравнение с именем каждого студента в векторе
	system("cls");
	string tmp;
	bool found = false;
	viewTitles();
	for (unsigned short i = 0; i < students.size(); i++) {
		student person = students[i];
		tmp = person.name;
		for (unsigned short i = 0; i < tmp.length(); i++)
			tmp[i] = tolower(tmp[i]);
		if (tmp.find(request) != string::npos) {
			viewStudent(&students[i]);
			found = true;
		}
	}

	if (!found) {
		system("cls");
		cout << "Nothing found" << endl; 
	}
	cout << "\npress any key...";
	_getwch();
	return 0;
}

int editStudent() {
	system("cls");
	// Пустой вектор
	if (students.size() == 0) {
		cout << "There is nothing to edit\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	// Запрос номера
	unsigned short id = getId();
	bool correct;
	while (true) {
		drawMenu(7, NAME, GROUP, MARKS, CREDITS, CIRCS, REMOVE, BACK);
		viewTitles();
		viewStudent(&students[id - 1]);
		cout << endl;
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
				if (!calculateCash(&students[id - 1]))
					return deleteStudent(id);
				break;
			// Зачеты
			case '4': setCredits(&students[id - 1]);
				calculateCash(&students[id - 1]);
				break;
			// Льготы
			case '5': setCircs(&students[id - 1]);
				calculateCash(&students[id - 1]);
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
	// Пустой вектор
	if (students.size() == 0) {
		system("cls");
		cout << "There is nothing to show\n" << endl;
		cout << "press any key...";
		_getwch();
		return 0;
	}
	drawMenu(5, BY_NO, BY_NAME, BY_CASH, BY_GPA, BACK);
	do switch (_getwch()) {
	// По номеру
	case '1': return viewList(students);
	// По имени
	case '2': return viewSortedList(students, byName);
	// По стипендии
	case '3': return viewSortedList(students, byCash);
	// По среднему баллу
	case '4': return viewSortedList(students, byGPA);
	// Вернуться
	case '0': return 0;
	} while (true);
}

int saveChanges() {
	// Запись из вектора в файл
	ofstream fout(DATA, ios::binary | ios::out | ios_base::trunc);
	for (student &person : students)
		fout.write((char*)&person, sizeof(student));
	fout.close();

	system("cls");
	cout << "All changes have been saved\n" << endl;
	cout << "press any key...";
	_getwch();

	return students.size();
}
