#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <map>
#include <conio.h>
#include "menu"
using namespace std;

typedef void(*function)();

int drawMenu(int num, ...) {
	va_list args;
	system("cls");

	// Top line
	int count = num;
	cout << (char)218;

	while (count--) {
		cout << setfill((char)196) << setw(4) 
			 << (char)194 << setw(13);

		if (count > 0)
			cout << (char)191 << " " << (char)218;
		else
			cout << (char)191 << endl;
	}

	// Middle line
	count = num;
	cout << (char)179;

	va_start(args, num);
	while (count--) {
		cout << " ";

		if (count == 0)
			cout << "0";
		else
			cout << num - count;

		cout << " " << setfill(' ') << (char)179 
			 << " " << setw(11) << left 
			 << va_arg(args, char*) << right;

		if (count > 0)
			cout << (char)179 << " " << (char)179;
		else
			cout << (char)179 << endl;
	}
	va_end(args);

	// Bottom line
	count = num;
	cout << (char)192;

	while (count--) {
		cout << setfill((char)196) << setw(4) 
			 << (char)193 << setw(13);

		if (count > 0)
			cout << (char)217 << " " << (char)192;
		else
			cout << (char)217 << endl;
	}

	return 0;
}

int chooseAction(int num, ...) {
	// MAD DEVELOPMENT

	va_list args;
	map<char, void(*)()> action;
	char key;

	va_start(args, num);
	for (int i = 1; i <= num; i++) {
		if (i == num)
			key = '0';
		else
			key = '0' + i;
		action.insert(pair<char, function>(key, va_arg(args, function)));
	}
	va_end(args);
	
	while (true) {
		key = _getch();
		if (action.count(key) > 0)
			action[key]();
	}

	return 0;
}