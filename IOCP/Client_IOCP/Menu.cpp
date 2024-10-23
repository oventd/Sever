
#include "Game.cpp"



void Game::menu()
{
		YELLOW
		printf("▒   ▒   ▒  ▒▒▒▒▒▒ ▒     ▒▒       ▒▒▒▒▒▒ ▒▒\n");
	printf("▒▒▒▒▒ ▒▒▒   ▒  ▒  ▒          ▒       ▒  ▒▒\n");
	printf("▒   ▒   ▒   ▒  ▒  ▒   ▒▒▒▒▒▒ ▒      ▒   ▒▒\n");
	printf("▒▒▒▒▒ ▒▒▒  ▒▒▒▒▒  ▒          ▒     ▒    ▒▒\n");
	printf("        ▒         ▒    ▒▒▒▒  ▒▒▒▒       ▒▒\n");
	printf("    ▒▒▒▒▒         ▒    ▒  ▒  ▒          ▒▒\n");
	printf("        ▒              ▒▒▒▒  ▒               \n");
	printf("    ▒▒▒▒▒                          \n");
	printf("    ▒                              \n");
	printf("    ▒▒▒▒▒                           \n");
}

void Game::move_menu(int* cursor1by, int* game_source1menu_change, int* game_source1score, int* game_source1score_ptf, int* game_source1begin_menu, int* game_source1diff)
{
	int kb1 = _getch();
	WHITE
		switch (kb1)
		{
		case DOWN:
			if (*cursor1by + 3 <= 23) {
				render.setClear(16, *cursor1by);
				render.setClear(32, *cursor1by);
				*cursor1by += 3;
			}

			break;

		case UP:
			if (*cursor1by - 3 >= 17) {
				render.setClear(16, *cursor1by);
				render.setClear(32, *cursor1by);
				*cursor1by -= 3;
			}
			break;

		case ENTER:

			if (*game_source1menu_change == 0 && *cursor1by == 17)
				*game_source1begin_menu = 1;
			else if (*game_source1menu_change == 0 && *cursor1by == 20)
			{
				//system("cls");
				*game_source1menu_change = 1;
			}
			else if (*game_source1menu_change == 0 && *cursor1by == 23)
				exit(0);
			else if (*game_source1menu_change == 1 && *cursor1by == 17)
			{
				*game_source1score_ptf = 1000;
				*game_source1diff = 0;
				*game_source1menu_change = 0;
			}

			else if (*game_source1menu_change == 1 && *cursor1by == 20)
			{
				*game_source1score_ptf = 3000;
				*game_source1diff = 1;
				*game_source1menu_change = 0;
			}

			else if (*game_source1menu_change == 1 && *cursor1by == 23)
			{
				*game_source1score_ptf = 6000;
				*game_source1diff = 2;
				*game_source1menu_change = 0;
			}

			break;
		}

}

void Game::Menu() {
	do
	{
		menu();
		render.gotoxy(20, 17);
		if (game_source1.start_re == 0)
		{
			LIGHT_BLUE printf(" 시작하기");
		}
		else
		{
			RED printf(" 다시하기");
		}

		BLUE render.printAt("난이도 설정", 20, 20);
		YELLOW render.printAt("  끝내기", 20, 23);


		while (1)
		{
			WHITE;
			render.printAt("▷", 16, cursor1.by);
			render.printAt("◁", 32, cursor1.by);
			move_menu(&cursor1.by, &game_source1.menu_change, &game_source1.score, &game_source1.score_ptf, &game_source1.begin_menu, &game_source1.diff);

			if (game_source1.menu_change == 1)//난이도 설정 눌렀을시 
			{
				system("cls");
				break;
			}
			else if (game_source1.begin_menu == 1)//시작하기 눌렀을시 
				break;

		}


		if (game_source1.menu_change == 1)//난이도 설정 눌렀을시
		{
			menu();
			gotoxy(20, 17);
			GRAY printf("   초급");
			gotoxy(20, 20);
			DARK_YELLOW printf("   중급");
			gotoxy(20, 23);
			BLOOD printf("   고급");
			WHITE
				cursor1.by = cursor1.by_last;

			gotoxy(16, cursor1.by);
			printf("▷");
			gotoxy(32, cursor1.by);
			printf("◁");


			while (1)
			{
				move_menu(&cursor1.by, &game_source1.menu_change, &game_source1.score, &game_source1.score_ptf, &game_source1.begin_menu, &game_source1.diff);
				if (game_source1.menu_change == 0)
				{
					system("cls");
					break;
				}
			}
			cursor1.by_last = cursor1.by;
			cursor1.by = 20;
		}
	} while (game_source1.begin_menu == 0);


	system("cls");
}

void Game::printSIdeInfo() {
	/*목차*/
	gotoxy(28, 2);
	PURPLE printf("난이도 : %s", game_source1.diff_ch);
	gotoxy(28, 6);
	LIGHT_GREEN printf("점수 : ");
	gotoxy(28, 8);
	RED printf("목숨 : ");
	gotoxy(28, 9);
	BLUE printf("승리 조건 :");
	gotoxy(40, 9);
	printf("%d점", game_source1.score_ptf);
	gotoxy(28, 11);
	GRAY printf(" 조작키");
	gotoxy(28, 13);
	printf(" ←  →");
}