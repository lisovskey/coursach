/*
* RitZEED inc.
*/

#include "stdafx"
#include "constants"
#include "stringer"
#include "console"
#include "presser"
#include "input"
#include "drawer"
#include "generator"
#include "student"
#include <fstream>
#include <vector>
#include <regex>
#include <algorithm>
using namespace std;

namespace {

	typedef struct {
		size_t	mathmatics	:4;
		size_t	programming	:4;
		size_t	physics		:4;
		size_t	philosophy	:4;
		size_t	analytics	:4;
	} marks;

	typedef struct {
		bool	graphics	:1;
		bool	english		:1;
		bool	pe			:1;
		bool	designing	:1;
		bool	history		:1;
	} credits;

	typedef struct {
		bool	budget		:1;
		bool	activism	:1;
		bool	science		:1;
		bool	foreign		:1;
		bool	invalid		:1;
		bool	dormitory	:1;
	} circumstances;

	typedef struct {
		size_t			id;
		char			name[STRING_LENGTH];
		size_t			group;
		marks			knowledge;
		double			gpa;
		credits			passes;
		circumstances	privileges;
		double			cash;
	} student;

	// Вектор студентов
	vector<student> students;

	// Изменяемые параметры
	double default_cash;
	double dormitory_rent_price;

	bool calculateCash(student* s) {
		/// Расчет стипендии

		double cash;
		bool satisfactory_marks = s->knowledge.mathmatics > 3.9 &&
			s->knowledge.programming > 3.9 &&
			s->knowledge.physics > 3.9 &&
			s->knowledge.philosophy > 3.9 &&
			s->knowledge.analytics > 3.9;
		bool passed_all_credits = s->passes.graphics &&
			s->passes.english &&
			s->passes.pe &&
			s->passes.designing &&
			s->passes.history;
		// Исключение
		if (!satisfactory_marks) {
			return false;
		}
		// Базовая стипендия
		if (s->privileges.foreign) {
			cash = default_cash * 2;
		}
		else if (s->privileges.budget) {
			if (!passed_all_credits) {
				cash = 0;
			}
			else cash = default_cash;
		}
		else {
			if (s->privileges.invalid)
				cash = default_cash * 0.8;
			else cash = 0;
		}
		// Множители
		if (cash > 1) {
			if (s->gpa < 5)
				cash *= 0.8;
			else if (s->gpa > 5.9 && s->gpa < 8)
				cash *= 1.2;
			else if (s->gpa > 7.9 && s->gpa < 9)
				cash *= 1.4;
			else if (s->gpa > 8.9)
				cash *= 1.6;

			if (s->privileges.invalid)
				cash *= 1.5;
			if (s->privileges.activism)
				cash *= 1.1;
			if (s->privileges.science)
				cash *= 1.2;
			if (s->privileges.dormitory)
				cash -= dormitory_rent_price;
		}
		// Если стипендия меньше платы за общежитие
		if (cash < 0) cash = 0;

		s->cash = cash;
		return true;
	}

	size_t getId() {
		/// Ввод номера с проверкой существования

		drawPreCentered(ENTER_ID, WINDOW_HEIGHT / 2);
		size_t id;
		while (true) {
			id = getPositiveNumber();
			if (id > 0 && id <= students.size())
				return id;
			else {
				clearScreen();
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_HEIGHT / 2);
				drawPreCentered(INVALID_ID, WINDOW_HEIGHT / 2);
			}
		}
	}

	void setName(student* s, const size_t y) {
		/// Ввод имени из двух слов только из букв

		drawPreCentered(ENTER_NAME, y);
		static char name[STRING_LENGTH + 1];
		while (true) {
			cin.getline(name, STRING_LENGTH + 1);
			cin.clear();
			if (strlen(name) == STRING_LENGTH) {
				cin.ignore(10000, '\n');
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(LONG_NAME, y);
			}
			else if (!regex_match(name, regex("[A-Z a-z]+"))) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(INVALID_NAME, y);
			}
			else if (wordCount(name) != 2) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(SURNAME_NAME, y);
			}
			else {
				strcpy_s(s->name, capitalize(name).c_str());
				return;
			}
		}
	}

	void setGroup(student* s, const size_t y) {
		/// Ввод шестизначного номера группы '16 года

		drawPreCentered(ENTER_GROUP, y);
		size_t group;
		while (true) {
			group = getPositiveNumber();
			if (group < 100000 || group > 999999) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(INVALID_GROUP, y);
			}
			else if (group < 600000 || group > 699999) {
				clsUnder(WINDOW_WIDTH, WINDOW_HEIGHT, y);
				drawPreCentered(DIFFERENT_GROUP, y);
			}
			else {
				s->group = group;
				return;
			}
		}
	}

	void setMarks(student* s, const size_t y) {
		/// Установка отметок с расчетом среднего балла

		double sum = 0;
		drawPreCentered(ENTER_MATH, y);
		sum += s->knowledge.mathmatics = getMark();
		drawPreCentered(ENTER_PROG, y + 1);
		sum += s->knowledge.programming = getMark();
		drawPreCentered(ENTER_PHYS, y + 2);
		sum += s->knowledge.physics = getMark();
		drawPreCentered(ENTER_PHIL, y + 3);
		sum += s->knowledge.philosophy = getMark();
		drawPreCentered(ENTER_ANAL, y + 4);
		sum += s->knowledge.analytics = getMark();
		s->gpa = sum / 5;
	}

	void setCredits(student* s, const size_t y) {
		/// Установка вовремя сданных зачетов

		drawPreCentered(ENTER_GRAPH, y);
		s->passes.graphics = getBoolean();
		drawPreCentered(ENTER_ENG, y + 1);
		s->passes.english = getBoolean();
		drawPreCentered(ENTER_PE, y + 2);
		s->passes.pe = getBoolean();
		drawPreCentered(ENTER_DESIGN, y + 3);
		s->passes.designing = getBoolean();
		drawPreCentered(ENTER_HIST, y + 4);
		s->passes.history = getBoolean();
	}

	void setCircs(student* s, const size_t y) {
		/// Установка влияющих факторов

		drawPreCentered(ENTER_BUDGET, y);
		s->privileges.budget = getBoolean();
		drawPreCentered(ENTER_ACTIVE, y + 1);
		s->privileges.activism = getBoolean();
		drawPreCentered(ENTER_SCIENCE, y + 2);
		s->privileges.science = getBoolean();
		drawPreCentered(ENTER_FOREIGN, y + 3);
		s->privileges.foreign = getBoolean();
		drawPreCentered(ENTER_INVALID, y + 4);
		s->privileges.invalid = getBoolean();
		drawPreCentered(ENTER_DORM, y + 5);
		s->privileges.dormitory = getBoolean();
	}

	size_t viewStudent(student* s) {
		/// Отображение основной информации о студенте

		// Расчет зачетов
		string passes = "";
		int count = 0;
		if (s->passes.graphics)		count++;
		if (s->passes.english)		count++;
		if (s->passes.pe)			count++;
		if (s->passes.designing)	count++;
		if (s->passes.history)		count++;
		passes += '0' + count;
		passes += "/5";

		// Другие факторы
		string circs = "";
		if (s->privileges.budget)		circs += "b ";
		if (s->privileges.activism)		circs += "a ";
		if (s->privileges.science)		circs += "s ";
		if (s->privileges.foreign)		circs += "f ";
		if (s->privileges.invalid)		circs += "i ";
		if (s->privileges.dormitory)	circs += "d";
		if (circs.length() == 0)		circs += "-";

		// Отображение
		cout << " " << fixed << right << setw(2) << setfill('0')
			<< s->id % 100 << left << " " << setfill(' ')
			<< (char)179 << setw(24) << s->name
			<< (char)179 << setw(8) << s->group << setprecision(1)
			<< (char)179 << setw(6) << s->gpa
			<< (char)179 << setw(8) << passes
			<< (char)179 << setw(11) << circs << setprecision(2)
			<< (char)179 << setw(13) << s->cash << endl;

		return s->id;
	}

	size_t createStudent() {		
		/// Создание студента вручную
		
		clearScreen();
		student s;
		s.id = students.size() + 1;

		if (s.id > 99) {
			drawCentered(TOO_MUCH_STUDENTS, WINDOW_HEIGHT / 2);
			waitAnyKey();
			return 0;
		}

		// Заполнение полей
		drawCentered(STUD_CREATING, 1);
		setName(&s, WINDOW_HEIGHT / 2 - 10);
		cout << endl;
		setGroup(&s, WINDOW_HEIGHT / 2 - 8);
		cout << endl;
		setMarks(&s, WINDOW_HEIGHT / 2 - 6);
		cout << endl;
		setCredits(&s, WINDOW_HEIGHT / 2);
		cout << endl;
		setCircs(&s, WINDOW_HEIGHT / 2 + 6);

		// В случае низких отметок
		if (!calculateCash(&s)) {
			clearScreen();
			drawCentered(STUDENT_EXPELLED, WINDOW_HEIGHT / 2);
			waitAnyKey();
			return 0;
		}

		// Добавление в вектор
		students.push_back(s);

		// Отображение
		drawTitles(7,
			4, " id", 24, "name", 8, "group", 6, "gpa",
			8, "passed", 11, "circs", 13, "cash");
		viewStudent(&s);
		drawCentered(STUDENT_ADDED, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return s.id;
	}

	size_t generateStudent() {
		/// Генерация студента

		clearScreen();
		student s;
		s.id = students.size() + 1;

		if (s.id > 99) {
			drawCentered(TOO_MUCH_STUDENTS, WINDOW_HEIGHT / 2);
			waitAnyKey();
			return 0;
		}

		strcpy_s(s.name, generateName().c_str());
		s.group = generateGroup();

		// Отметки
		s.gpa = 0;
		s.gpa += s.knowledge.mathmatics = generateMark();
		s.gpa += s.knowledge.programming = generateMark();
		s.gpa += s.knowledge.physics = generateMark();
		s.gpa += s.knowledge.philosophy = generateMark();
		s.gpa += s.knowledge.analytics = generateMark();
		s.gpa /= 5;

		// Другие факторы
		s.privileges.invalid = generateBool(-16);
		s.privileges.budget = generateBool((int)s.gpa / 4);
		s.privileges.foreign = generateBool(-8 - 4 * s.privileges.invalid);
		s.privileges.activism = generateBool(-2 + s.privileges.budget);
		s.privileges.science = generateBool(-5 + (int)(s.gpa + s.privileges.invalid) / 3);
		s.privileges.dormitory = generateBool(1 - 3 * !s.privileges.budget + 
			s.privileges.invalid + s.privileges.activism + s.privileges.foreign);

		// Своевременные зачеты
		s.passes.graphics = generateBool((int)s.gpa + 8 * s.privileges.budget - 1);
		s.passes.designing = generateBool((int)s.gpa + 8 * s.privileges.budget - 1);
		s.passes.english = generateBool((int)s.gpa + 8 * s.privileges.budget - 1);
		s.passes.pe = generateBool((int)s.gpa + 8 * s.privileges.budget - 1);
		s.passes.history = generateBool((int)s.gpa + 8 * s.privileges.budget - 1);

		calculateCash(&s);

		// Добавление в вектор
		students.push_back(s);

		drawTitles(7,
			4, " id", 24, "name", 8, "group", 6, "gpa",
			8, "passed", 11, "circs", 13, "cash");
		viewStudent(&s);
		drawCentered(STUDENT_GENERATED, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return s.id;
	}

	size_t deleteStudent(const size_t id) {
		/// Удаление студента из вектора

		clearScreen();
		students.erase(students.begin() + id - 1);
		// Исправление номеров
		for (size_t i = id - 1; i < students.size(); i++) {
			students[i].id = i + 1;
		}
		drawCentered(STUDENT_GENERATED, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return students.size();
	}

	size_t viewList(vector<student> &list) {
		/// Отображение всех студентов в переданном векторе

		size_t size = students.size();
		size_t height = WINDOW_HEIGHT - 4;
		size_t limit = 0;
		TConsole tc;
		for (size_t j = 0; j < size; j += height) {
			drawTitles(7,
				4, " id", 24, "name", 8, "group", 6, "gpa",
				8, "passed", 11, "circs", 13, "cash");

			limit += (size - j < height) ? size % height : height;

			for (size_t i = j; i < limit; i++) {
				viewStudent(&list[i]);
			}
			waitAnyKey();
			clearScreen();
		}

		return list.size();
	}

	size_t viewSortedList(vector<student> list, bool(*compare)(student, student)) {
		/// Сортировка вектора по переданной функции

		sort(list.begin(), list.end(), compare);
		return viewList(list);
	}

	bool byName(student a, student b) {
		/// Сортировка по убыванию имени

		return strcmp(a.name, b.name) < 0;
	}

	bool byCash(student a, student b) {
		/// Сортировка по убыванию стипендии

		return a.cash > b.cash;
	}

	bool byGPA(student a, student b) {
		/// Сортировка по убыванию среднего балла

		return a.gpa > b.gpa;
	}

	size_t viewFoundStudents(bool(*condition)(student, string)) {
		/// Поиск и отображение найденных по запросу студентов

		clearScreen();
		// Ввод запроса
		string request;
		drawPreCentered(SEARCH, WINDOW_HEIGHT / 2);
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
						4, " id", 24, "name", 8, "group", 6, "gpa",
						8, "passed", 11, "circs", 13, "cash");
					found = true;
				}
				count++;
				viewStudent(&students[i]);
			}
		}

		if (!found) {
			clearScreen();
			drawCentered(NOTHING_FOUND, WINDOW_HEIGHT / 2);
		}
		else cout << "\n" << endl;
		waitAnyKey();
		return count;
	}

	bool byName(student s, string request) {
		/// Поиск по имени

		return lower(s.name).find(request) != string::npos;
	}

	bool byGroup(student s, string request) {
		/// Поиск по группе

		return to_string(s.group).find(request) != string::npos;
	}

	bool readStudents() {
		/// Считывание студентов из файла в вектор

		ifstream studin(STUDLIST, ios::binary | ios::in);
		if (studin.is_open()) {
			size_t i = 1;
			student tmp;
			while (studin.peek() != EOF) {
				studin.read((char*)&tmp, sizeof(student));
				calculateCash(&tmp);
				tmp.id = i++;
				students.push_back(tmp);
			}
			studin.close();
			return students.size() != 0;
		}
		else {
			return false;
		}
	}

	bool readConfig() {
		/// Считывание натроек из файла

		ifstream confin(CONFLIST, ios::in);

		if (confin.is_open()) {
			confin.seekg(strlen(DEFAULT_CASH_CONFIG), confin.cur);
			if (confin.cur != EOF) {
				confin >> default_cash;
				confin.ignore(10000, '\n');
			}
			else {
				confin.close();
				return false;
			}
			confin.seekg(strlen(DORM_PRICE_CONFIG), confin.cur);
			if (confin.cur != EOF) {
				confin >> dormitory_rent_price;
			} 
			else {
				confin.close();
				return false;
			}
		} else return false;

		confin.close();
		return true;
	}

	void changeDefaultCash() {
		/// Смена базовой стипендии

		clearScreen();
		drawPreCentered(CURRENT_CASH, 1);
		cout << fixed << setprecision(2) << default_cash;
		drawPreCentered(ENTER_VALUE, WINDOW_HEIGHT / 2);
		default_cash = getMoney();

		// Пересчет стипендии
		for (student &person : students) {
			calculateCash(&person);
		}

		clearScreen();
		drawCentered(CASH_CHANGED, WINDOW_HEIGHT / 2);
		waitAnyKey();
	}

	void changeDormitoryRentPrice() {
		/// Смена стоимости оплаты общежития

		clearScreen();
		drawPreCentered(CURRENT_PRICE, 1);
		cout << fixed << setprecision(2) << dormitory_rent_price;
		drawPreCentered(ENTER_VALUE, WINDOW_HEIGHT / 2);
		dormitory_rent_price = getMoney();

		// Пересчет стипендии
		for (student &person : students) {
			calculateCash(&person);
		}

		clearScreen();
		drawCentered(PRICE_CHANGED, WINDOW_HEIGHT / 2);
		waitAnyKey();
	}

}

void readData() {
	/// Считывание студентов и настроек с дефолтными значениями

	readStudents();
	if (!readConfig()) {
		default_cash = 58.28;
		dormitory_rent_price = 18.22;
	}
}

size_t addStudent() {
	/// Добавление нового студента

	clearScreen();
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
	/// Поиск студента по определенному полю

	clearScreen();
	// Пустой вектор
	if (students.size() == 0) {
		drawCentered(NOTHING_TO_SEARCH, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	while (true) {
		drawCentered(SEARCHING, 1);
		drawMenu(3, BY_NAME, BY_GROUP, BACK);
		do {
			correct_press = true;
			switch (getPress()) {
			// По имени
			case '1': return viewFoundStudents(byName);
			// По группе
			case '2': return viewFoundStudents(byGroup);
			// Вернуться
			case '0': return 0;
			// Неверный ввод
			default: correct_press = false;
			}
		} while (true);
	}
}

size_t editStudent() {
	/// Изменение полей студента

	clearScreen();
	// Пустой вектор
	if (students.size() == 0) {
		drawCentered(NOTHING_TO_EDIT, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	// Запрос номера
	size_t id = getId();
	while (true) {
		// Изменяемый студент
		drawTitles(7,
			4, " id", 24, "name", 8, "group", 6, "gpa",
			8, "passed", 11, "circs", 13, "cash");
		viewStudent(&students[id - 1]);
		drawMenu(7, NAME, GROUP, MARKS, CREDITS, CIRCS, REMOVE, BACK);
		do {
			correct_press = true;
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
			default: correct_press = false;
			}
		} while (!correct_press);
	}
}

size_t viewStudents() {
	/// Отображение всех студентов

	// Пустой вектор
	if (students.size() == 0) {
		clearScreen();
		drawCentered(NOTHING_TO_SHOW, WINDOW_HEIGHT / 2);
		waitAnyKey();
		return 0;
	}
	return viewList(students);
}

size_t sortStudents() {
	/// Отображение всех студентов в определенном порядке

	clearScreen();
	// Пустой вектор
	if (students.size() == 0) {
		drawCentered(NOTHING_TO_SORT, WINDOW_HEIGHT / 2);
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

void writeStudents() {
	/// Запись из вектора в файл

	ofstream studout(STUDLIST, ios::binary | ios::out | ios_base::trunc);
	for (student &person : students)
		studout.write((char*)&person, sizeof(student));
	studout.close();
}

void writeConfig() {
	/// Запись настроек в файл

	ofstream confout(CONFLIST, ios::out | ios_base::trunc);
	confout << DEFAULT_CASH_CONFIG << default_cash << "\n";
	confout << DORM_PRICE_CONFIG << dormitory_rent_price;
	confout.close();
}

size_t settings() {
	/// Настройки значений

	clearScreen();
	drawCentered(SETTINGS, 1);
	drawMenu(3, DEFAULT_CASH, DORM_PRICE, BACK);
	do {
		correct_press = true;
		switch (getPress()) {
		// Изменить базовую стипендию
		case '1': changeDefaultCash();
			return 1;
		// Изменить стоимость оплаты общежития
		case '2': changeDormitoryRentPrice();
			return 2;
		// Вернуться
		case '0': return 0;
		// Неверный ввод
		default: correct_press = false;
		}
	} while (true);
}
