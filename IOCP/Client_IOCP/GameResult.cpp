#include "Game.cpp"


void Game::win() {
	gotoxy(5, 10);
	BLUE printf("  게임 승리");
	gotoxy(5, 11);
	printf("ESC - 돌아가기");

	do
	{
		kb1 = _getch();
	} while (kb1 != ESC);
	system("cls");
	game_source1.start_re = 1;
}
void Game::lose() {
	gotoxy(5, 10);
	RED printf("  게임 오버");
	gotoxy(5, 11);
	printf("ESC - 돌아가기");
	do
	{
		kb1 = _getch();
	} while (kb1 != ESC);

	system("cls");

	game_source1.start_re = 1;
}