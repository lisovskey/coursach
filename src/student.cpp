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
		cout << "Enter id: ";
		size_t id;
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
		size_t group;
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

	size_t getMark() {
		// Валидная отметка
		size_t mark;
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

	size_t createStudent() {
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

		s.id = (size_t)students.size() + 1;

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

	size_t generateStudent() {
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
		s.privileges.budget = generateBool((int)(s.gpa + 2) / 3);
		s.privileges.invalid = generateBool(-16);
		s.privileges.activism = generateBool(-3 + s.privileges.budget);
		s.privileges.science = generateBool((int)(s.gpa + s.privileges.invalid) / 4);
		s.privileges.foreign = generateBool(-6 - 4 * s.privileges.invalid);
		s.privileges.dormitory = generateBool(2 + s.privileges.invalid + s.privileges.activism);

		// Своевременные зачеты
		s.passes.graphics = generateBool((int)s.gpa + 5 * s.privileges.budget);
		s.passes.designing = generateBool((int)s.gpa + 5 * s.privileges.budget);
		s.passes.english = generateBool((int)s.gpa + 5 * s.privileges.budget);
		s.passes.swimming = generateBool((int)s.gpa + 5 * s.privileges.budget);
		s.passes.history = generateBool((int)s.gpa + 5 * s.privileges.budget);

		s.id = (size_t)students.size() + 1;
		calculateCash(&s);

		// Добавление в вектор
		students.push_back(s);
		cout << "Student has been added" << endl;
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
		cout << "Student was deleted" << endl;
		waitAnyKey();
		return students.size();
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
		if (s->privileges.foreign)
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

	size_t viewList(vector<student> &list) {
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

size_t findStudent() {
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
	for (size_t i = 0; i < request.length(); i++)
		request[i] = tolower(request[i]);

	// Сравнение с именем каждого студента в векторе
	system("cls");
	string name;
	size_t count = 0;
	bool found = false;
	for (size_t i = 0; i < students.size(); i++) {
		student person = students[i];
		name = lower(person.name);
		if (name.find(request) != string::npos) {
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

	if (!found)
		cout << "Nothing found" << endl; 
	else cout << endl;
	waitAnyKey();
	return count;
}

size_t editStudent() {
	system("cls");
	// Пустой вектор
	if (students.size() == 0) {
		cout << "There is nothing to edit" << endl;
		waitAnyKey();
		return 0;
	}
	// Запрос номера
	size_t id = getId();
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

size_t viewStudents() {
	// Пустой вектор
	if (students.size() == 0) {
		system("cls");
		cout << "There is nothing to show" << endl;
		waitAnyKey();
		return 0;
	}
	return viewList(students);
}

size_t sortStudents() {
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

size_t saveStudents() {
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
