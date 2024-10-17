#include "header.h"

class Render {
public:
	void gotoxy(int x, int y) {
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}
	void setClear(int x, int y)
	{
		gotoxy(x, y);
		printf("  ");
	}
	
	void printAt(const char*msg, int x, int y)
	{
		gotoxy(x, y);
		printf(msg);
	}



};