#include <cstring>
#include <cstdarg>
#include <windows.h>
#include "olives"

int wordCount(char* str) {
	int count = 0;
	int len = strlen(str);
	for (int i = 0; i <= len; i++)
		if (str[i] == ' ' || str[i] == '\0')
			if (str[i + 1] != ' ' || i == len)
				count++;
	return count;
}

template <typename T>
double average(int num, ...) {
	va_list args;
	if (num == 0) {
		cerr << "No arguments" << endl;
		exit(1);
	}

	double sum = 0;
	__try {
		va_start(args, num);
		while (num--) {
			sum += va_arg(args, T);
		}
		va_end(args);
	}
	__except (GetExceptionCode() == EXCEPTION_ACCESS_VIOLATION ?
		EXCEPTION_EXECUTE_HANDLER :
		EXCEPTION_CONTINUE_SEARCH) {
		cerr << "Memory error" << endl;
		exit(2);
	}

	return sum / num;
}