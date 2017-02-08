/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <windows.h>
#include "console"
#include "drawer"
using namespace std;

TConsole tc;

void drawMenu(size_t num, ...) {
	if (num > 7) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	va_list args;
	system("cls");
	size_t count = num;

	cout << left;
	__try {
		va_start(args, num);
		while (count--) {
			tc.InvertColors();

			if (count == 0)
				cout << " 0 ";
			else
				cout << " " << num - count << " ";

			tc.InvertColors();

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
}

void drawTitles(size_t num, ...) {
	if (num > 10) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	va_list args;
	size_t count = num;

	__try {
		tc.InvertColors();
		va_start(args, num);
		size_t width = va_arg(args, size_t);
		cout << left << setw(width) << va_arg(args, char*);
		while (--count) {
			width = va_arg(args, size_t);
			cout << (char)179 << setw(width) << va_arg(args, char*);
		}
		va_end(args);
		tc.InvertColors();
		cout << endl;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER :
		EXCEPTION_CONTINUE_SEARCH) {
		tc.InvertColors();
		cerr << "Memory error" << endl;
		exit(2);
	}
}
