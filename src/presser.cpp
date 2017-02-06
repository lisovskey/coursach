#include <conio.h>
#include <iostream>
#include "presser"
using namespace std;

char getPress() {
	return (char)_getwch();
}

void waitAnyKey() {
	std::cout << "\npress any key...";
	getPress();
}
