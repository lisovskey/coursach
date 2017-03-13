/*
* RitZEED inc.
*/

#include "stdafx"
#include "stringer"
using namespace std;

size_t wordCount(string str) {
	size_t count = 0;
	size_t len = str.length();
	for (size_t i = 1; i <= len; i++) {
		if (str[i] == ' ') {
			if (str[i - 1] != ' ') {
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

string capitalize(string str) {
	size_t len = str.length();
	if (str[0] != ' ') str[0] = toupper(str[0]);
	for (size_t i = 1; i < len; i++) {
		if (str[i] != ' ') {
			if (str[i - 1] == ' ') {
				str[i] = toupper(str[i]);
			}
			else {
				str[i] = tolower(str[i]);
			}
		}
	}
	return str;
}
