/*
* RitZEED inc.
*/

#include "stdafx"
#include "drawer"
#include "presser"
#include <conio.h>
using namespace std;

int getPress() {
	return _getwch();
}

void waitAnyKey() {
	drawPressAnyKey();
	getPress();
}
