/*
* RitZEED inc.
*/

#include <string>
#include "stringer"

unsigned wordCount(std::string str) {
	unsigned count = 0;
	unsigned len = str.length();
	for (unsigned i = 0; i <= len; i++) {
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

std::string lower(std::string str) {
	for (unsigned i = 0; i < str.length(); i++) {
		str[i] = tolower(str[i]);
	}
	return str;
}
