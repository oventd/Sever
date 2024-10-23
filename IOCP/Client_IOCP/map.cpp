#include "Game.cpp"
void Game::gotoxy(int x, int y)
{
	COORD Cur;
	Cur.X = x;
	Cur.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void Game::map_ptf(int k) {
	
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 12; j++) {
			switch (map[k][i][j])
			{
			case 0:
				printf("  "); break;
			case 1:
				PLUM printf("бр"); break;
			case 2:
				YELLOW printf("в╞"); break;
			}
		}
		putchar('\n');
	}
}