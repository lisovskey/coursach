#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>
#include "generator"
using namespace std;

string generateName() {
	srand((unsigned int)time(NULL));
	string name = "";
	name += NAMES[rand() % 102];
	name += " ";
	name += SURNAMES[rand() % 5];
	return name;
}

unsigned int generateGroup() {
	srand((unsigned int)time(NULL));
	return GROUPS[rand() % 92];
}