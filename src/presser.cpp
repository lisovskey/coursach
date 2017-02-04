#include <conio.h>
#include <iostream>

char getPress() {
	return (char)_getwch();
}

void waitAnyKey() {
	std::cout << "\npress any key...";
	getPress();
}
