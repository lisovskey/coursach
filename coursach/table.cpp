#include <iostream>
#include <iomanip>
#include <cstdarg>
#include "table.h"
using namespace std;

int drawRow(int width, ...) {
	va_list args;

	// Top line
	int num = width;
	cout << " " << (char)218;

	while (num--) {
		cout << setfill((char)196) << setw(4) 
			 << (char)194 << setw(12);

		if (num > 0)
			cout << (char)191 << " " << (char)218;
		else
			cout << (char)191 << endl;
	}

	// Middle line
	num = width;
	cout << " " << (char)179;

	va_start(args, width);
	while (num--) {
		cout << " ";

		if (num == 0)
			cout << "0";
		else
			cout << width - num;

		cout << " " << setfill(' ') << (char)179 
			 << " " << setw(10) << left 
			 << va_arg(args, char*) << right;

		if (num > 0)
			cout << (char)179 << " " << (char)179;
		else
			cout << (char)179 << endl;
	}
	va_end(args);

	// Bottom line
	num = width;
	cout << " " << (char)192;

	while (num--) {
		cout << setfill((char)196) << setw(4) 
			 << (char)193 << setw(12);

		if (num > 0)
			cout << (char)217 << " " << (char)192;
		else
			cout << (char)217 << endl;
	}

	return 0;
}