/*
*
* RitZEED inc.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "table"

void dosmth() {}

int main(int argc, char* argv[]) {
	drawRow(5, "Add", "Edit", "Sort", "Save", "Quit");
	// Main loop
	while (true) {
		// Choose action loop
		do switch (_getch()) {
		// TODO
		case '1': dosmth();
			break;
		// TODO
		case '2': dosmth();
			break;
		// TODO
		case '3': dosmth();
			break;
		// TODO
		case '4': dosmth();
			break;
		// TODO
		case '0': dosmth();
			return 0;
		// Incorrect key
		default: continue;
		} while (false);
	}
}