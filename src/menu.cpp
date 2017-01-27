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
	if (num > 7) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	FlushConsoleInputBuffer(hConsole);
	va_list args;
	system("cls");
	int count = num;

	cout << left;
	__try {
		va_start(args, num);
		while (count--) {
			SetConsoleTextAttribute(hConsole, 
				BACKGROUND_RED | BACKGROUND_GREEN |
				BACKGROUND_BLUE | BACKGROUND_INTENSITY);
			if (count == 0)
				cout << " 0 ";
			else
				cout << " " << num - count << " ";
			SetConsoleTextAttribute(hConsole, 
				FOREGROUND_RED | FOREGROUND_GREEN |
				FOREGROUND_BLUE | FOREGROUND_INTENSITY);

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
