/*
* RitZEED inc.
*/

#include <cstring>
#include <iostream>
#include <conio.h>
#include <windows.h>
#include "olives"

int wordCount(char* str) {
	int count = 0;
	int len = strlen(str);
	for (int i = 0; i <= len; i++) {
		if (str[i] == ' ') {
			if (str[i + 1] != ' ') {
				count++;
			}
		}
		else if (str[i] == '\0') {
			if (str[i - 1] != ' ') {
				count++;
			}
		}
	}
	return count;
}

int waitEscape() {
	while (true)
		if (GetAsyncKeyState(VK_ESCAPE))
			return 0;
}

void waitAnyKey() {
	std::cout << "\npress any key...";
	_getwch();
}
