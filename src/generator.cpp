#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>
#include "generator"
using namespace std;

string generateName() {
	string name = "";
	name += NAMES[rand() % 102];
	name += " ";
	name += SURNAMES[rand() % 35];
	return name;
}

unsigned int generateGroup() {
	return GROUPS[rand() % 92];
}

unsigned short generateMark() {
	return rand() % 7 + 4;
}

bool generateBool(int factor) {
	return factor > 0 ? rand() % factor : !(rand() % -factor);
}