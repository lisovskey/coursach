/*
* RitZEED inc.
*/

#include <string>
#include <cstdlib>
#include <random>
#include "generator"
using namespace std;

random_device rd;
mt19937 mt(rd());

string generateName() {
	string name = "";
	uniform_int_distribution<int> dist_name(0, 101);
	name += NAMES[dist_name(mt)];
	name += " ";
	uniform_int_distribution<int> dist_surname(0, 34);
	name += SURNAMES[dist_surname(mt)];
	return name;
}

size_t generateGroup() {
	uniform_int_distribution<int> dist(0, 90);
	return GROUPS[dist(mt)];
}

size_t generateMark() {
	uniform_int_distribution<int> dist(4, 10);
	return dist(mt);
}

bool generateBool(int factor) {
	uniform_int_distribution<int> dist(0, abs(factor));
	return factor >= 0 ? dist(mt) != 0 : dist(mt) == 0;
}
