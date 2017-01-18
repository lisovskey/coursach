/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <windows.h>
#include "menu"
using namespace std;

int drawMenu(int num, ...) {
	if (num > 10) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}
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

	__try {
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
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
		EXCEPTION_EXECUTE_HANDLER : 
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << "Memory error" << endl;
		exit(2);
	}

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

	return num;
}