/*
* RitZEED inc.
*/

#include <string>
#include "stringer"
using namespace std;

size_t wordCount(string str) {
	size_t count = 0;
	size_t len = str.length();
	for (size_t i = 0; i <= len; i++) {
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

string lower(string str) {
	for (size_t i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}
