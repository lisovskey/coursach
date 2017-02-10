/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <cstring>
#include <windows.h>
#include "constants"
#include "console"
#include "drawer"
using namespace std;

TConsole tc;

void drawVerticalLine(size_t length, size_t x, size_t y) {
	while (length--) {
		tc.GotoXY(x, y++);
		cout << (char)179;		
	}
}

void drawColumn(size_t width) {
	if (width % 2) width++;
	drawVerticalLine(WINDOW_HEIGHT, WINDOW_WIDTH / 2 - width / 2, 0);
	drawVerticalLine(WINDOW_HEIGHT, WINDOW_WIDTH / 2 + width / 2 - 1, 0);
}

void drawMenu(size_t num, ...) {
	if (num > 10) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	va_list args;
	system("cls");
	size_t x = WINDOW_WIDTH / 2 - 5;
	size_t y = WINDOW_HEIGHT / 2 - num;
	size_t count = num;

	cout << left;
	__try {
		va_start(args, num);
		while (count--) {
			tc.GotoXY(x, y);
			tc.InvertColors();

			if (count == 0)
				cout << " 0 ";
			else
				cout << " " << num - count << " ";

			tc.InvertColors();

			cout << " " << setfill(' ') << setw(8) << va_arg(args, char*);
			y += 2;
		}
		va_end(args);
		tc.GotoXY(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
		EXCEPTION_EXECUTE_HANDLER : 
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << "Memory error" << endl;
		exit(2);
	}
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

void drawPressAnyKey() {
	tc.GotoXY(WINDOW_WIDTH / 2 - 8, WINDOW_HEIGHT - 2);
	cout << "press any key...";
	tc.GotoXY(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
}

void drawCentered(const char* str) {
	tc.GotoXY(WINDOW_WIDTH / 2 - strlen(str) / 2, WINDOW_HEIGHT / 2);
	cout << str;
	tc.GotoXY(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
}

void drawPreCentered(const char* str, size_t y) {
	tc.GotoXY(WINDOW_WIDTH / 2 - strlen(str), y);
	cout << str;
}
