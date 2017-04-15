#include "stdafx"
#include "console"

TConsole::TConsole() :Out(std::cout)
{
	OutputHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
}

void TConsole::DelLine()
{
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
	CHAR_INFO CI;
	ScreenBufInfo.srWindow.Top = WhereY();
	CI.Char.UnicodeChar = ' ';
	CI.Attributes = ScreenBufInfo.wAttributes;
	COORD Coord;
	Coord.X = ScreenBufInfo.srWindow.Left;
	Coord.Y = ScreenBufInfo.srWindow.Top - 1;
	DWORD Size, Count;
	Size = ScreenBufInfo.srWindow.Right - ScreenBufInfo.srWindow.Left + 1;
	ScrollConsoleScreenBuffer(OutputHandle, &ScreenBufInfo.srWindow, 0, Coord, &CI);
	FillConsoleOutputAttribute(OutputHandle, ScreenBufInfo.wAttributes, Size, Coord, &Count);
}

void TConsole::GotoXY(int X, int Y)
{
	ScreenBufInfo.dwCursorPosition.X = X;
	ScreenBufInfo.dwCursorPosition.Y = Y;
	if (!SetConsoleCursorPosition(OutputHandle, ScreenBufInfo.dwCursorPosition)) {
		GotoXY(0, 0);
		DelLine();
	}
}

void TConsole::BackgroundColor(BYTE Color)
{
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
	WORD TextAttr = ScreenBufInfo.wAttributes;
	TextAttr = (Color << 4) | (TextAttr & 0x0F);
	SetConsoleTextAttribute(OutputHandle, TextAttr);
}

void TConsole::TextColor(BYTE Color)
{
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
	WORD TextAttr = ScreenBufInfo.wAttributes;
	TextAttr = (Color & 0x0F) | (TextAttr & 0xF0);
	SetConsoleTextAttribute(OutputHandle, TextAttr);
}

void TConsole::InvertColors()
{
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
	BYTE TextAttr = (BYTE)ScreenBufInfo.wAttributes;
	TextAttr = ~TextAttr;
	SetConsoleTextAttribute(OutputHandle, TextAttr);
}

int TConsole::WhereX()
{
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
	return ScreenBufInfo.dwCursorPosition.X;
}

int TConsole::WhereY()
{
	GetConsoleScreenBufferInfo(OutputHandle, &ScreenBufInfo);
	return ScreenBufInfo.dwCursorPosition.Y;
}

void TConsole::Window(BYTE X, BYTE Y)
{
	SMALL_RECT ConsoleRect;
	ConsoleRect.Top = 0;
	ConsoleRect.Left = 0;
	ConsoleRect.Right = X - 1;
	ConsoleRect.Bottom = Y - 1;
	COORD BufSize;
	BufSize.X = X;
	BufSize.Y = Y;
	SetConsoleScreenBufferSize(OutputHandle, BufSize);
	SetConsoleWindowInfo(OutputHandle, TRUE, &ConsoleRect);
	GotoXY(0, 0);
}

void TConsole::initWindow(BYTE width, BYTE height, const char* title)
{
	if (width + height > 255) {
		std::cerr << "Too big window" << std::endl;
		exit(1);
	}
	TConsole tc;
	tc.Window(width, height);
	tc.Window(width, height);
	SetConsoleTitle(title);
	tc.BackgroundColor(COLOR_BLACK);
	tc.TextColor(COLOR_WHITE);
}

void TConsole::clsUnder(BYTE width, BYTE height, size_t y)
{
	TConsole tc;
	tc.GotoXY(0, y);
	int count = height - y;
	if (count > 0) {
		std::cout << std::setw(width * count) << std::setfill(' ') << "";
	}
}
