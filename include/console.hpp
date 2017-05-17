#pragma once

#include <windows.h>

enum {
	COLOR_BLACK,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_BROWN,
	COLOR_LIGHTGRAY,
	COLOR_DARKGRAY,
	COLOR_LIGHTBLUE,
	COLOR_LIGHTGREEN,
	COLOR_LIGHTCYAN,
	COLOR_LIGHTRED,
	COLOR_LIGHTMAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE
};

class TConsole {
private:
	HANDLE OutputHandle;
	CONSOLE_SCREEN_BUFFER_INFO ScreenBufInfo;
public:
	std::ostream& Out;
	TConsole();
	void DelLine();
	void GotoXY(int, int);
	void BackgroundColor(BYTE);
	void TextColor(BYTE);
	void InvertColors();
	int WhereX();
	int WhereY();
	void Window(BYTE, BYTE);
	static void initWindow(BYTE, BYTE, const char*);
	static void clsUnder(BYTE, BYTE, size_t);
};
