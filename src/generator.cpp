/*
* RitZEED inc.
*/

#include <string>
#include <cstdlib>
#include "generator"
using namespace std;

string generateName() {
	string name = "";
	name += NAMES[rand() % 102];
	name += " ";
	name += SURNAMES[rand() % 35];
	return name;
}

size_t generateGroup() {
	return GROUPS[rand() % 92];
}

size_t generateMark() {
	return rand() % 7 + 4;
}

bool generateBool(int factor) {
	return factor > 0 ? rand() % factor != 0 : rand() % factor == 0;
}
