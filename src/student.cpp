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
#include "stringer"
#include "console"
#include "presser"
#include "input"
#include "drawer"
#include "generator"
#include "student"
using namespace std;

namespace {
	// Внутрефайловые имена

	typedef struct {
		size_t	math : 4;
		size_t	prog : 4;
		size_t	phys : 4;
		size_t	phil : 4;
	} marks;

	typedef struct {
		bool			graphics : 1;
		bool			english : 1;
		bool			swimming : 1;
		bool			designing : 1;
		bool			history : 1;
	} credits;

	typedef struct {
		bool			budget : 1;
		bool			activism : 1;
		bool			science : 1;
		bool			foreign : 1;
		bool			invalid : 1;
		bool			dormitory : 1;
	} circumstances;

	typedef struct {
		size_t	id;
		char			name[24];
		size_t		group;
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
		double cash, start_cash = 58.28;
		bool satisfactory_marks = s->knowledge.math >= 4 &&
			s->knowledge.prog >= 4 &&
			s->knowledge.phys >= 4 &&
			s->knowledge.phil >= 4;
		bool passed_all_credits = s->passes.graphics &&
			s->passes.english &&
			s->passes.swimming &&
			s->passes.designing &&
			s->passes.history;
		// Исключение
		if (!satisfactory_marks) {
			return false;
		}
		// Базовая стипендия
		if (s->privileges.foreign) {
			cash = start_cash * 2;
		}
		else if (s->privileges.budget) {
			if (!passed_all_credits) {
				cash = 0;
			}
			else cash = start_cash;
		}
		else {
			if (s->privileges.invalid)
				cash = start_cash * 0.8;
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

	size_t getId() {
		// Проверка существования
		drawPreCentered("Enter id: ", WINDOW_HEIGHT / 2);
		size_t id;
		while (true) {
			id = getPositiveNumber(WINDOW_HEIGHT / 2);
			if (id > 0 && id <= students.size())
				return id;
			else {
				system("cls");
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEIGHT / 2);
				drawPreCentered("Invalid id: ", WINDOW_HEIGHT / 2);
			}
		}
	}

	void setName(student* s, size_t y) {
		// Имя из двух слов только из букв
		drawPreCentered("Enter name: ", y);
		static char name[25];
		while (true) {
			cin.getline(name, 25);
			cin.clear();
			if (strlen(name) == 24) {
				cin.ignore(10000, '\n');
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Too long name: ", y);
			}
			else if (!regex_match(name, regex("[A-Z a-z]+"))) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Name and surname: ", y);
			}
			else if (wordCount(name) != 2) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Invalid name: ", y);
			}
			else {
				strcpy_s(s->name, name);
				return;
			}
		}
	}

	void setGroup(student* s, size_t y) {
		// Шестизначный номер группы
		drawPreCentered("Enter group: ", y);
		size_t group;
		while (true) {
			group = getPositiveNumber(y);
			if (group > 99999 && group < 1000000) {
				s->group = group;
				return;
			}
			else {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Invalid group: ", y);
			}
		}
	}

	size_t getMark(size_t y) {
		// Валидная отметка
		size_t mark;
		while (true) {
			mark = getPositiveNumber(y);
			if (mark > 0 && mark < 11) {
				return mark;
			}
			else {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered("Invalid mark: ", y);
			}
		}
	}

	void setMarks(student* s, size_t y) {
		// Установка отметок с расчетом среднего балла
		double sum = 0;
		drawPreCentered("Enter math mark: ", y);
		sum += s->knowledge.math = getMark(y);
		drawPreCentered("Enter programming mark: ", y + 1);
		sum += s->knowledge.prog = getMark(y + 1);
		drawPreCentered("Enter physics mark: ", y + 2);
		sum += s->knowledge.phys = getMark(y + 2);
		drawPreCentered("Enter philosophy mark: ", y + 3);
		sum += s->knowledge.phil = getMark(y + 3);
		s->gpa = sum / 4;
	}

	void setCredits(student* s, size_t y) {
		// Установка вовремя сданных зачетов
		drawPreCentered("Passed graphics on time: ", y);
		s->passes.graphics = getBoolean(y);
		drawPreCentered("Passed english on time: ", y + 1);
		s->passes.english = getBoolean(y + 1);
		drawPreCentered("Passed swimming on time: ", y + 2);
		s->passes.swimming = getBoolean(y + 2);
		drawPreCentered("Passed designing on time: ", y + 3);
		s->passes.designing = getBoolean(y + 3);
		drawPreCentered("Passed history on time: ", y + 4);
		s->passes.history = getBoolean(y + 4);
	}

	void setCircs(student* s, size_t y) {
		// Установка влияющих факторов
		drawPreCentered("Is on budget: ", y);
		s->privileges.budget = getBoolean(y);
		drawPreCentered("Is activist: ", y + 1);
		s->privileges.activism = getBoolean(y + 1);
		drawPreCentered("Do scince: ", y + 2);
		s->privileges.science = getBoolean(y + 2);
		drawPreCentered("Is foreign: ", y + 3);
		s->privileges.foreign = getBoolean(y + 3);
		drawPreCentered("Is invalid: ", y + 4);
		s->privileges.invalid = getBoolean(y + 4);
		drawPreCentered("Lives in dormitory: ", y + 5);
		s->privileges.dormitory = getBoolean(y + 5);
	}

	size_t viewStudent(student* s) {
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
		if (s->privileges.invalid)
			circs += "i ";
		if (s->privileges.dormitory)
			circs += "d";
		if (circs.length() == 0)
			circs += "-";

		// Отображение
		cout << right << setfill('0') << setw(2)
			<< s->id << setfill(' ') << setw(1) << "" << left
			<< (char)179 << setw(24) << s->name
			<< (char)179 << setw(9) << s->group
			<< (char)179 << setw(8) << s->gpa
			<< (char)179 << setw(8) << passes
			<< (char)179 << setw(11) << circs
			<< (char)179 << setw(11) << fixed
			<< setprecision(2) << s->cash << endl;

		return s->id;
	}

	size_t createStudent() {
		// Создание студента
		system("cls");
		drawCentered(STUD_CREATING, 1);
		student s;
		s.id = students.size() + 1;

		setName(&s, WINDOW_HEIGHT / 2 - 10);
		cout << endl;
		setGroup(&s, WINDOW_HEIGHT / 2 - 8);
		cout << endl;
		setMarks(&s, WINDOW_HEIGHT / 2 - 6);
		cout << endl;
		setCredits(&s, WINDOW_HEIGHT / 2 - 1);
		cout << endl;
		setCircs(&s, WINDOW_HEIGHT / 2 + 5);

		// В случае низких отметок
		if (!calculateCash(&s)) {
			system("cls");
			drawCentered("Student cannot study here", WINDOW_HEIGHT / 2);
			waitAnyKey();
			return 0;
		}

		// Добавление в вектор
		students.push_back(s);
		drawTitles(7,
			3, "id", 24, "name", 9, "group", 8, "gpa",
			8, "passed", 11, "circs", 11, "cash");
		viewStudent(&s);
		drawCentered("Student has been added", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return s.id;
	}

	size_t generateStudent() {
		// Генерация студента
		system("cls");
		student s;
		s.id = students.size() + 1;

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
		s.privileges.invalid = generateBool(-16);
		s.privileges.budget = generateBool((int)s.gpa / 4);
		s.privileges.foreign = generateBool(-8 - 4 * s.privileges.invalid);
		s.privileges.activism = generateBool(-2 + s.privileges.budget);
		s.privileges.science = generateBool(-5 + (int)(s.gpa + s.privileges.invalid) / 3);
		s.privileges.dormitory = generateBool(1 - 3 * !s.privileges.budget + 
			s.privileges.invalid + s.privileges.activism + s.privileges.foreign);

		// Своевременные зачеты
		s.passes.graphics = generateBool((int)s.gpa + 6 * s.privileges.budget - 1);
		s.passes.designing = generateBool((int)s.gpa + 6 * s.privileges.budget - 1);
		s.passes.english = generateBool((int)s.gpa + 6 * s.privileges.budget - 1);
		s.passes.swimming = generateBool((int)s.gpa + 6 * s.privileges.budget - 1);
		s.passes.history = generateBool((int)s.gpa + 6 * s.privileges.budget - 1);

		calculateCash(&s);

		// Добавление в вектор
		students.push_back(s);
		drawTitles(7,
			3, "id", 24, "name", 9, "group", 8, "gpa",
			8, "passed", 11, "circs", 11, "cash");
		viewStudent(&s);
		drawCentered("Student has been generated", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return s.id;
	}

	size_t deleteStudent(size_t id) {
		// Удаление студента из вектора
		system("cls");
		students.erase(students.begin() + id - 1);
		// Исправление номеров
		for (size_t i = id - 1; i < students.size(); i++) {
			students[i].id = i + 1;
		}
		drawCentered("Student was deleted", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return students.size();
	}

	size_t viewList(vector<student> &list) {
		// Заголовки
		drawTitles(7,
			3, "id", 24, "name", 9, "group", 8, "gpa",
			8, "passed", 11, "circs", 11, "cash");
		// Основная информация о каждом студенте
		for (student &s : list) {
			viewStudent(&s);
		}
		cout << "\n" << endl;
		waitAnyKey();

		return list.size();
	}

	size_t viewSortedList(vector<student> list, bool(*compare)(student, student)) {
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

	size_t viewFoundStudents(bool(*condition)(student, string)) {
		system("cls");
		// Ввод запроса
		string request;
		drawPreCentered("search: ", WINDOW_HEIGHT / 2);
		getline(cin, request);
		cin.clear();
		request = lower(request);

		// Сравнение с именем каждого студента в векторе		
		size_t count = 0;
		bool found = false;
		for (size_t i = 0; i < students.size(); i++) {
			student s = students[i];
			if (condition(s, request)) {
				if (!found) {
					drawTitles(7,
						3, "id", 24, "name", 9, "group", 8, "gpa",
						8, "passed", 11, "circs", 11, "cash");
					count++;
					found = true;
				}
				viewStudent(&students[i]);
			}
		}

		if (!found) {
			system("cls");
			drawCentered("Nothing found", WINDOW_HEIGHT / 2);
		}
		else cout << "\n" << endl;
		waitAnyKey();
		return count;
	}

	bool byName(student s, string request) {
		return lower(s.name).find(request) != string::npos;
	}

	bool byGroup(student s, string request) {
		return to_string(s.group).find(request) != string::npos;
	}
}

size_t getStudents() {
	// Считывание из файла в вектор
	ifstream fin(STUDLIST, ios::binary | ios::in);
	if (fin.is_open()) {
		size_t i = 1;
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

size_t addStudent() {
	system("cls");
	drawCentered(ADDING, 1);
	drawMenu(3, MANUAL, GENERATE, BACK);
	do switch (getPress()) {
	// Добавить вручную
	case '1': return createStudent();
	// Сгенерировать
	case '2': return generateStudent();
	// Вернуться
	case '0': return 0;
	} while (true);
}

size_t findStudent() {
	system("cls");
	// Пустой вектор
	if (students.size() == 0) {
		drawCentered("There is nothing to look for", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	bool correct;
	while (true) {
		drawCentered(SEARCHING, 1);
		drawMenu(3, BY_NAME, BY_GROUP, BACK);
		do {
			correct = true;
			switch (getPress()) {
			// По имени
			case '1': return viewFoundStudents(byName);
			// По группе
			case '2': return viewFoundStudents(byGroup);
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: correct = false;
			}
		} while (true);
	}
}

size_t editStudent() {
	system("cls");
	// Пустой вектор
	if (students.size() == 0) {
		drawCentered("There is nothing to edit", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	// Запрос номера
	size_t id = getId();
	bool correct;
	while (true) {
		drawTitles(7,
			3, "id", 24, "name", 9, "group", 8, "gpa",
			8, "passed", 11, "circs", 11, "cash");
		viewStudent(&students[id - 1]);
		drawMenu(7, NAME, GROUP, MARKS, CREDITS, CIRCS, REMOVE, BACK);
		do {
			correct = true;
			switch (getPress()) {
			// Имя
			case '1': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setName(&students[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Группу
			case '2': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setGroup(&students[id - 1], WINDOW_HEIGHT / 2);
				break;
			// Отметки
			case '3': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setMarks(&students[id - 1], WINDOW_HEIGHT / 2 - 2);
				if (!calculateCash(&students[id - 1]))
					return deleteStudent(id);
				break;
			// Зачеты
			case '4': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setCredits(&students[id - 1], WINDOW_HEIGHT / 2 - 2);
				calculateCash(&students[id - 1]);
				break;
			// Льготы
			case '5': clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, 2);
				setCircs(&students[id - 1], WINDOW_HEIGHT / 2 - 3);
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

size_t viewStudents() {
	// Пустой вектор
	if (students.size() == 0) {
		system("cls");
		drawCentered("There is nothing to show", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	return viewList(students);
}

size_t sortStudents() {
	system("cls");
	// Пустой вектор
	if (students.size() == 0) {
		drawCentered("There is nothing to sort", WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	drawCentered(SORTING, 1);
	drawMenu(4, BY_NAME, BY_CASH, BY_GPA, BACK);
	do switch (getPress()) {
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

size_t saveStudents() {
	// Запись из вектора в файл
	ofstream fout(STUDLIST, ios::binary | ios::out | ios_base::trunc);
	for (student &person : students)
		fout.write((char*)&person, sizeof(student));
	fout.close();

	system("cls");
	drawCentered("All changes have been saved", WINDOW_HEIGHT / 2);
	waitAnyKey();

	return students.size();
}
