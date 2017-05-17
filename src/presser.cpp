/*
* RitZEED inc.
*/

#include "stdafx.hpp"
#include "drawer.hpp"
#include "presser.hpp"
#include <conio.h>

void waitAnyKey()
{
	drawPressAnyKey();
	getPress();
}
