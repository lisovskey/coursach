/*
* RitZEED inc.
*/

#include "stdafx.hpp"
#include "stringer.hpp"
using namespace std;

string lower(string str)
{
	for (char &symbol : str) {
		symbol = tolower(symbol);
	}
	return str;
}

string capitalize(string str)
{
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
