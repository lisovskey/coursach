#include <conio.h>
#include "drawer"
#include "presser"
using namespace std;

int getPress() {
	return _getwch();
}

void waitAnyKey() {
	drawPressAnyKey();
	getPress();
}
