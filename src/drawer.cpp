/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <windows.h>
#include "drawer"
using namespace std;

HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int drawMenu(unsigned num, ...) {
	if (num > 7) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	va_list args;
	system("cls");
	int count = num;

	CONSOLE_SCREEN_BUFFER_INFO bckp;
	GetConsoleScreenBufferInfo(hConsole, &bckp);
	unsigned char prev_color = (unsigned char)bckp.wAttributes;
	unsigned char new_color = ~prev_color;

	cout << left;
	__try {
		va_start(args, num);
		while (count--) {
			SetConsoleTextAttribute(hConsole, new_color);
			if (count == 0)
				cout << " 0 ";
			else
				cout << " " << num - count << " ";
			SetConsoleTextAttribute(hConsole, prev_color);

			cout << " " << setfill(' ') << setw(7) << va_arg(args, char*);
		}
		va_end(args);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
		EXCEPTION_EXECUTE_HANDLER : 
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << "Memory error" << endl;
		exit(2);
	}
	cout << "\n" << endl;

	return num;
}

int drawTitles(unsigned num, ...) {
	if (num > 10) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	va_list args;
	int count = num;

	CONSOLE_SCREEN_BUFFER_INFO bckp;
	GetConsoleScreenBufferInfo(hConsole, &bckp);
	unsigned char prev_color = (unsigned char)bckp.wAttributes;
	unsigned char new_color = ~prev_color;
	SetConsoleTextAttribute(hConsole, new_color);

	__try {
		va_start(args, num);
		unsigned width = va_arg(args, unsigned);
		cout << left << setw(width) << va_arg(args, char*);
		while (--count) {
			width = va_arg(args, unsigned);
			cout << (char)179 << setw(width) << va_arg(args, char*);
		}
		va_end(args);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER :
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << "Memory error" << endl;
		exit(2);
	}
	SetConsoleTextAttribute(hConsole, prev_color);
	cout << endl;

	return num;
}
