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
int auth();
int userMode();
int adminMode();

int main(int argc, char* argv[]) {
	return auth() ? userMode() : adminMode();
}

int auth() {
	system("cls");
	drawRow(2, "User", "Admin");
	// Auth loop
	while (true) {
		// Choose action loop
		do switch (_getch()) {
			// TODO
		case '1': dosmth();
			return 1;
			// TODO
		case '0': dosmth();
			return 0;
			// Incorrect key
		default: continue;
		} while (false);
	}
}

int userMode() {
	system("cls");
	drawRow(3, "Find", "Sort", "Quit");
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
		case '0': dosmth();
			return 0;
		// Incorrect key
		default: continue;
		} while (false);
	}
}

int adminMode() {
	system("cls");
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