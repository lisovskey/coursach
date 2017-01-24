/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <windows.h>
#include "menu"
using namespace std;

HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int drawMenu(int num, ...) {
	if (num > 10) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	FlushConsoleInputBuffer(hConsole);
	SetConsoleTextAttribute(hConsole, 240);
	va_list args;
	system("cls");
	int count = num;

	cout << left;
	__try {
		va_start(args, num);
		while (count--) {
			SetConsoleTextAttribute(hConsole, 15);
			if (count == 0)
				cout << " 0 ";
			else
				cout << " " << num - count << " ";
			SetConsoleTextAttribute(hConsole, 240);

			cout << " " << setfill(' ') << setw(11) << va_arg(args, char*);
		}
		va_end(args);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
		EXCEPTION_EXECUTE_HANDLER : 
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << "Memory error" << endl;
		exit(2);
	}
	cout << endl;

	return num;
}