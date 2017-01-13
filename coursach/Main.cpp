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
		// Choose action loop
		do switch (getch()) {
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
		// Exit program
		case '0': return 0;
		// Incorrect key
		default: continue;
		} while (false);
	}
}