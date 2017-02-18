/*
* RitZEED inc.
*/

#include <iostream>
#include <iomanip>
#include <cstdarg>
#include <cstring>
#include <string>
#include <windows.h>
#include "constants"
#include "console"
#include "drawer"
using namespace std;

TConsole tc;

template <typename T>
void drawButton(T button) {
	tc.InvertColors();
	cout << " " << button << " ";
	tc.InvertColors();
}

void drawMenu(size_t num, ...) {
	if (num > 10) {
		cerr << "Too many arguments" << endl;
		exit(1);
	}

	va_list args;
	size_t x = WINDOW_WIDTH / 2 - 5;
	size_t y = WINDOW_HEIGHT / 2 - num;
	size_t count = num;

	cout << left;
	__try {
		va_start(args, num);
		while (count--) {
			tc.GotoXY(x, y);

			if (count == 0)
				drawButton("0");
			else
				drawButton(num - count);

			cout << " " << va_arg(args, char*);
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

	system("cls");
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
	drawCentered("press any key...", WINDOW_HEIGHT - 2);
	tc.GotoXY(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
}

void drawCentered(const char* str, size_t y) {
	tc.GotoXY(WINDOW_WIDTH / 2 - strlen(str) / 2, y);
	cout << " " << str << " ";
}

void drawPreCentered(const char* str, size_t y) {
	tc.GotoXY(0, y);
	cout << setw(WINDOW_WIDTH / 2) << setfill(' ') << right << str;
}

void drawHelp() {
	system("cls");
	drawCentered("copyright 2017 lisovskey", 1);
	drawCentered("english and windows only", 3);
	drawCentered("bsuir bdprog course work, variant 23", 4);
	drawCentered("console program for calculating students' grants", 5);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered("coursach", 6);
	drawCentered("feel free to generate and manually create students", 9);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(ADD, 10);
	drawCentered("to edit fields and delete them", 13);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(EDIT, 14);
	drawCentered("to search by name or group", 17);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(FIND, 18);
	drawCentered("to sort by name, gpa and grant", 21);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(SORT, 22);
	drawCentered("and to simply view in id order", 25);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(VIEW, 26);
	drawCentered("also to create and edit accounts", 29);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(ACCS, 30);
	drawCentered("and don't forget to save changes", 33);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered(SAVE, 34);
}
