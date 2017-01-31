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
#include <algorithm>
#include "constants"
#include "olives"
#include "input"
#include "drawer"
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
	unsigned		group;
	marks			knowledge;
	double			gpa;
	credits			passes;
	circumstances	privileges;
	double			cash;
} student;

// Вектор студентов
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

unsigned getStudents() {
	// Считывание из файла в вектор
	ifstream fin(STUDLIST, ios::binary | ios::in);
	if (fin.is_open()) {
		unsigned i = 1;
		student tmp;
		while (fin.peek() != EOF) {
			fin.read((char*)&tmp, sizeof(student));
			calculateCash(&tmp);
			tmp.id = i++;
			students.push_back(tmp);
		}
		fin.close();
		return students.size();
	}
	else {
		return 0;
	}
}

namespace {
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
}

void setName(student* s) {
	// Имя из двух слов только из букв
	cout << "Enter name: ";
	static char name[25];
	while (true) {
		cin.getline(name, 25);
		cin.clear();
		if (strlen(name) == 24) {
			cin.ignore(10000, '\n');
			cout << "Too long name: ";
		}
		else if (regex_match(name, regex("[A-Z a-z]+"))) {
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
	unsigned group;
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
	unsigned mark;
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

unsigned short createStudent() {
	// Создание студента
	system("cls");
	student s;

	setName(&s);
	cout << endl;
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
		cout << "\nStudent cannot study here" << endl;
		waitAnyKey();
		return 0;
	}

	// Добавление в вектор
	students.push_back(s);
	cout << "\nStudent has been added" << endl;
	waitAnyKey();
	return s.id;
}

unsigned short generateStudent() {
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
	s.gpa = s.gpa / 4;

	// Другие факторы
	s.privileges.budget = generateBool((int)(s.gpa + 2) / 3);
	s.privileges.invalid = generateBool(-12);
	s.privileges.activism = generateBool(-3 + s.privileges.budget);
	s.privileges.science = generateBool((int)(s.gpa + s.privileges.invalid) / 4);
	s.privileges.foreign = generateBool(-5 - 2 * s.privileges.invalid);
	s.privileges.dormitory = generateBool(2 + s.privileges.invalid + s.privileges.activism);
	
	// Своевременные зачеты
	s.passes.graphics = generateBool((int)s.gpa + 5 * s.privileges.budget);
	s.passes.designing = generateBool((int)s.gpa + 5 * s.privileges.budget);
	s.passes.english = generateBool((int)s.gpa + 5 * s.privileges.budget);
	s.passes.swimming = generateBool((int)s.gpa + 5 * s.privileges.budget);
	s.passes.history = generateBool((int)s.gpa + 5 * s.privileges.budget);

	s.id = (unsigned short)students.size() + 1;
	calculateCash(&s);

	// Добавление в вектор
	students.push_back(s);
	cout << "Student has been added" << endl;
	waitAnyKey();
	return s.id;
}

int deleteStudent(unsigned short id) {
	// Удаление студента из вектора
	system("cls");
	students.erase(students.begin() + id - 1);
	// Исправление номеров
	for (unsigned i = id - 1; i < students.size(); i++) {
		students[i].id = i + 1;
	}
	cout << "Student was deleted" << endl;
	waitAnyKey();
	return -1;
}

unsigned short viewStudent(student* s) {
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
		<< (char)179 << setw(9) << s->group
		<< (char)179 << setw(8) << s->gpa
		<< (char)179 << setw(8) << passes
		<< (char)179 << setw(11) << circs
		<< (char)179 << setw(11) << fixed
		<< setprecision(2) << s->cash << endl;

	return s->id;
}

unsigned viewList(vector<student> &list) {
	system("cls");
	// Заголовки
	drawTitles(7, 
		3, "id", 24, "name", 9, "group", 8, "gpa",
		8, "passed", 11, "circs", 11, "cash");
	// Основная информация о каждом студенте
	for (student &s : list) {
		viewStudent(&s);
	}
	cout << endl;	
	waitAnyKey();

	return list.size();
}

unsigned viewSortedList(vector<student> list, bool (*compare)(student, student)) {
	// Сортировка по переданной функции
	sort(list.begin(), list.end(), compare);
	//bsort(list, compare);
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

unsigned short addStudent() {
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
		cout << "There is nothing to look for" << endl;
		waitAnyKey();
		return 0;
	}
	// Ввод запроса
	string request;
	cout << "search: ";
	getline(cin, request);
	cin.clear();
	for (unsigned i = 0; i < request.length(); i++)
		request[i] = tolower(request[i]);

	// Сравнение с именем каждого студента в векторе
	system("cls");
	string tmp;
	bool found = false;
	drawTitles(7,
		3, "id", 24, "name", 9, "group", 8, "gpa",
		8, "passed", 11, "circs", 11, "cash");
	for (unsigned i = 0; i < students.size(); i++) {
		student person = students[i];
		tmp = person.name;
		for (unsigned i = 0; i < tmp.length(); i++)
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
	else cout << endl;
	waitAnyKey();
	return 0;
}

int editStudent() {
	system("cls");
	// Пустой вектор
	if (students.size() == 0) {
		cout << "There is nothing to edit" << endl;
		waitAnyKey();
		return 0;
	}
	// Запрос номера
	unsigned short id = getId();
	bool correct;
	while (true) {
		drawMenu(7, NAME, GROUP, MARKS, CREDITS, CIRCS, REMOVE, BACK);
		drawTitles(7,
			3, "id", 24, "name", 9, "group", 8, "gpa",
			8, "passed", 11, "circs", 11, "cash");
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

unsigned viewStudents() {
	// Пустой вектор
	if (students.size() == 0) {
		system("cls");
		cout << "There is nothing to show" << endl;
		waitAnyKey();
		return 0;
	}
	return viewList(students);
}

unsigned sortStudents() {
	// Пустой вектор
	if (students.size() == 0) {
		system("cls");
		cout << "There is nothing to sort" << endl;
		waitAnyKey();
		return 0;
	}
	drawMenu(4, BY_NAME, BY_CASH, BY_GPA, BACK);
	do switch (_getwch()) {
	// По имени
	case '1': return viewSortedList(students, byName);
	// По стипендии
	case '2': return viewSortedList(students, byCash);
	// По среднему баллу
	case '3': return viewSortedList(students, byGPA);
	// Вернуться
	case '0': return 0;
	} while (true);
}

unsigned saveStudents() {
	// Запись из вектора в файл
	ofstream fout(STUDLIST, ios::binary | ios::out | ios_base::trunc);
	for (student &person : students)
		fout.write((char*)&person, sizeof(student));
	fout.close();

	system("cls");
	cout << "All changes have been saved" << endl;
	waitAnyKey();

	return students.size();
}
