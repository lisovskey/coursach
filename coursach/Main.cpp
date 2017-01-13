/*
*
* RitZEED inc.
*
*/

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

void dosmth() {}

int main(int argc, char* argv[]) {
	// Main loop
	while (true) {
		bool again = false;

		// Choose action loop
		do switch (_getch()) {

		case '1': dosmth();
			break;

		case '2': dosmth();
			break;

		case '3': dosmth();
			break;

		case '0': return 0;
		// Exit program
		default: again = true;
		} while (again);
	}
}