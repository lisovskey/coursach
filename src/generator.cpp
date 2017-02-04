/*
* RitZEED inc.
*/

#include <string>
#include <cstdlib>
#include "generator"

std::string generateName() {
	std::string name = "";
	name += NAMES[rand() % 102];
	name += " ";
	name += SURNAMES[rand() % 35];
	return name;
}

unsigned generateGroup() {
	return GROUPS[rand() % 92];
}

unsigned short generateMark() {
	return rand() % 7 + 4;
}

bool generateBool(int factor) {
	return factor > 0 ? rand() % factor != 0 : rand() % factor == 0;
}
