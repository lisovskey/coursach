/*
*
* RitZEED inc.
*
*/

#include <conio.h>
#include "constants"
#include "menu"
#include "student"

int auth();
int userMode();
int adminMode();
int quit();

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
		drawMenu(2, ADMIN, USER);
		// Choose action loop
		do switch (_getch()) {
			correct = true;
		// Admin mode
		case '1': dosmth();
			return 1;
		// User mode
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
		drawMenu(3, FIND, SORT, QUIT);
		// Choose action loop
		do switch (_getch()) {
			correct = true;
		// TODO
		case '1': findStudent();
			break;
		// TODO
		case '2': sortStudents();
			break;
		// Quit program
		case '0': return 0;
		// Incorrect key
		default: correct = false;
		} while (!correct);
	}
}

int adminMode() {
	// Main loop
	while (true) {
		drawMenu(5, ADD, EDIT, SORT, SAVE, QUIT);
		// Choose action loop
		do switch (_getch()) {
			correct = true;
		// TODO
		case '1': addStudent();
			break;
		// TODO
		case '2': editStudent();
			break;
		// TODO
		case '3': sortStudents();
			break;
		// TODO
		case '4': dosmth();
			break;
		// Quit program
		case '0': return quit();
		// Incorrect key
		default: correct = false;
		} while (!correct);
	}
}

int quit() {
	// Quit loop
	while (true) {
		drawMenu(2, SAVE_N_QUIT, QUIT);
		// Choose action loop
		do switch (_getch()) {
			correct = true;
			// Save changes and quit
		case '1': saveChanges();
			// Quit without saving changes
		case '0': return 0;
			// Incorrect key
		default: correct = false;
		} while (!correct);
	}
}