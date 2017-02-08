#include <conio.h>
#include <iostream>
#include "presser"
using namespace std;

int getPress() {
	return _getwch();
}

void waitAnyKey() {
	std::cout << "\npress any key...";
	getPress();
}
