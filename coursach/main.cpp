/*
*
* RitZEED inc.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "menu"
using namespace std;

struct student {
	string name;
	int group;
	double gpa;
	bool budget;
	bool social;
	int cash;
};

int auth();
int userMode();
int adminMode();

// Temp function
void dosmth() {}

// Menu loops helper
bool correct;

int main(int argc, char* argv[]) {
	return auth() ? adminMode() : userMode();
}

int auth() {
	// Auth loop
	while (true) {
		drawMenu(2, "Admin", "User");
		// Choose action loop
		do switch (_getch()) {
			correct = true;
		// TODO
		case '1': dosmth();
			return 1;
		// TODO
		case '0': dosmth();
			return 0;
		// Incorrect key
		default: correct = false;
		} while (!correct);
	}
}

int userMode() {
	// Main loop
	while (true) {
		drawMenu(3, "Find", "Sort", "Quit");
		// Choose action loop
		do switch (_getch()) {
			correct = true;
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
		default: correct = false;
		} while (!correct);
	}
}

int adminMode() {
	// Main loop
	while (true) {
		drawMenu(5, "Add", "Edit", "Sort", "Save", "Quit");
		// Choose action loop
		do switch (_getch()) {
			correct = true;
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
		default: correct = false;
		} while (!correct);
	}
}