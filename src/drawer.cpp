/*
* RitZEED inc.
*/

#include "stdafx"
#include "constants"
#include "console"
#include "drawer"
#include <cstdarg>
using namespace std;

const string TOO_MANY_ARGS = "Too many arguments";
const string MEMORY_ERROR = "Memory error";

TConsole tc = TConsole();

template <typename T>
void drawButton(T button)
{
	tc.InvertColors();
	cout << " " << button << " ";
	tc.InvertColors();
}

void drawMenu(const size_t num, ...)
{
	if (num > 10) {
		cerr << TOO_MANY_ARGS << endl;
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
				drawButton(0);
			else
				drawButton(num - count);

			cout << " " << va_arg(args, string);
			y += 2;
		}
		va_end(args);
		tc.GotoXY(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ? 
		EXCEPTION_EXECUTE_HANDLER : 
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << MEMORY_ERROR << endl;
		exit(2);
	}
}

void drawTitles(const size_t num, ...)
{
	if (num > 10) {
		cerr << TOO_MANY_ARGS << endl;
		exit(1);
	}

	clearScreen();
	va_list args;
	size_t count = num;

	__try {
		tc.InvertColors();
		va_start(args, num);
		size_t width = va_arg(args, size_t);
		cout << left << setw(width) << va_arg(args, string);
		while (--count) {
			width = va_arg(args, size_t);
			cout << (char)179 << setw(width) << va_arg(args, string);
		}
		va_end(args);
		tc.InvertColors();
		cout << endl;
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER :
		EXCEPTION_CONTINUE_SEARCH) {
		tc.InvertColors();
		cerr << MEMORY_ERROR << endl;
		exit(2);
	}
}

void drawPressAnyKey()
{
	drawCentered(PRESS_ANY_KEY, WINDOW_HEIGHT - 2);
	tc.GotoXY(WINDOW_WIDTH - 1, WINDOW_HEIGHT - 1);
}

void drawCentered(const string str, const size_t y)
{
	tc.GotoXY(WINDOW_WIDTH / 2 - str.length() / 2, y);
	cout << " " << str << " ";
}

void drawPreCentered(const string str, const size_t y)
{
	tc.GotoXY(0, y);
	cout << setw(WINDOW_WIDTH / 2) << setfill(' ') << right << str;
}

void drawHelp(bool admin)
{
	clearScreen();
	drawCentered("copyright 2017 lisovskey", 1);
	drawCentered("english and windows only", 3);
	drawCentered("bsuir bdprog course work, variant 23", 4);
	drawCentered("console program for calculating students' grants", 5);
	cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
	drawCentered("coursach", 6);
	if (admin) {
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
		drawCentered("and just to view in id order", 25);
		cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
		drawCentered(VIEW, 26);
		drawCentered("also to create and edit accounts", 29);
		cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
		drawCentered(ACCS, 30);
		drawCentered("and don't forget to save changes", 33);
		cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
		drawCentered(SAVE, 34);
	}
	else {
		drawCentered("feel free to search by name or group", 10);
		cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
		drawCentered(FIND, 11);
		drawCentered("to sort by name, gpa and grant", 14);
		cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
		drawCentered(SORT, 15);
		drawCentered("and just to view in id order", 18);
		cout << endl << setw(WINDOW_WIDTH) << setfill((char)196) << "";
		drawCentered(VIEW, 19);
		drawCentered("there are all weird letters you need to know", 23);
		drawPreCentered("b - ", 24);
		cout << "budget";
		drawPreCentered("a - ", 25);
		cout << "activism";
		drawPreCentered("s - ", 26);
		cout << "science";
		drawPreCentered("f - ", 27);
		cout << "foreign";
		drawPreCentered("i - ", 28);
		cout << "invalid";
		drawPreCentered("d - ", 29);
		cout << "dormitory";
	}
}
