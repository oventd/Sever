#include "Game.cpp"

void Game::razer(int* razer_x, int* razer_y, int* razer_switch, int* razer_index, int* cursor1cx, int* cursor1cy, int* game_source1score){
	if (*razer_index == 1) {
		do{
			*razer_y = rand() % 19 + 2;
		} while (((*razer_y) % 2 == 0 ? 1 : 0) == 0);
	}

	if (*razer_index > 4 && *razer_index < 11) {
		if (*razer_index % 2 == 0) {
			for (int i = 1; i < 18; i++) {
				gotoxy(*razer_y, i);

				printf("  ");
			}
		}
		else {
			for (int i = 1; i < 19; i++){
				if (i % 2 == 1){
					gotoxy(*razer_y, i);

					RED printf("¦­");
				}
			}
		}
	}

	else if (*razer_index >= 11){
		if (*razer_x < 19){
			gotoxy(*razer_y, *razer_x);
			RED printf("¡á");
			(*razer_x)++;
		}
		if (*razer_x >= 19){
			gotoxy(*razer_y, *razer_x - 1);
			RED printf("¡á");
		}

		if (*razer_index > 24)	{
			for (int i = 1; i < 19; i++){
				gotoxy(*razer_y, i);

				printf("  ");
			}
			gotoxy(*cursor1cx, *cursor1cy);

			BLUE_GREEN printf("%s", "¿ô");

			*razer_x = 1;

			*game_source1score += 10;

			*razer_switch = 0;
			*razer_index = 0;
			*razer_y = 0;
		}
		gotoxy(48, 26);
	}





}

void Game::star(int* n, int* star_x, int* razer_y, int* heart_y, int* cursor1cx, int* cursor1cy, int* game_source1score){
	if (*n == 1){
		int do_switch = 0;
		do{
			*star_x = rand() % 19 + 2;
			if (*star_x != *razer_y && *star_x % 2 == 0) {
				do_switch = 1;
			}
		} while (do_switch == 0);
	}

	if (*n < 19){
		if (*n != 1){
			gotoxy(*star_x, *n - 1);
			printf("  ");
		}
		gotoxy(*star_x, *n);
		YELLOW printf("¡Ú");
		(*n)++;
	}

	else{
		gotoxy(*star_x, *n - 1);
		printf("  ");

		gotoxy(*cursor1cx, *cursor1cy);

		BLUE_GREEN printf("%s", "¿ô");

		*n = 1;

		*game_source1score += 5;
	}
	gotoxy(48, 26);
}

void Game::heart(int* heart_x, int* heart_y, int* razer_y, int* heart_switch, int* cursor1cx, int* cursor1cy, int* game_source1score) //±èµµÈÆ ±â´É ÇÔ¼ö
{
	if (*heart_x == 1) {
		int do_switch = 0;
		do
		{
			*heart_y = rand() % 19 + 2;
			if (*heart_y != *razer_y && *heart_y % 2 == 0) {
				do_switch = 1;
			}
		} while (do_switch == 0);
	}

	if (*heart_x < 19)
	{
		if (*heart_x != 1)		{
			gotoxy(*heart_y, *heart_x - 1);
			printf("  ");
		}
		gotoxy(*heart_y, *heart_x);
		GREEN printf("¢¾");

		(*heart_x)++;
	}

	else
	{
		gotoxy(*heart_y, *heart_x - 1);
		printf("  ");

		gotoxy(*cursor1cx, *cursor1cy);
		BLUE_GREEN printf("%s", "¿ô");
		*heart_x = 1;
		*game_source1score += 5;
		*heart_switch = 0;
	}
	gotoxy(48, 26);
}

//class Razer : public Fall {
//private:
//	int switch_state;
//	int index;
//
//public:
//	Razer(int cx, int cy, int score, int idx, int sw) : Fall(cx, cy, score), index(idx), switch_state(sw) {}
//
//	void fallStep() override {
//		if (index == 1) {
//			do {
//				position_x = rand() % 19 + 2;
//			} while (position_x % 2 != 0);
//		}
//
//		if (index > 4 && index < 11) {
//			if (index % 2 == 0) {
//				for (int i = 1; i < 18; i++) {
//					gotoxy(position_x, i);
//					printf("  ");
//				}
//			}
//			else {
//				for (int i = 1; i < 19; i++) {
//					if (i % 2 == 1) {
//						gotoxy(position_x, i);
//						RED printf("¦­");
//					}
//				}
//			}
//		}
//		else if (index >= 11) {
//			if (position_y < 19) {
//				gotoxy(position_x, position_y);
//				RED printf("¡á");
//				position_y++;
//			}
//
//			if (index > 24) {
//				resetPosition();
//				switch_state = 0;
//				index = 0;
//			}
//		}
//		gotoxy(48, 26);
//	}
//};
//
//class Star : public Fall {
//public:
//	Star(int cx, int cy, int score) : Fall(cx, cy, score) {
//		shape = "¡Ú";
//	}
//
//	void fallStep() override {
//		updatePosition();
//		if (position_y == 19) {
//			resetPosition();
//			game_score += 5;
//		}
//	}
//};
//
//class Heart : public Fall {
//private:
//	int switch_state;
//	int razer_position_x;
//
//public:
//	Heart(int cx, int cy, int score, int sw, int razer_x) : Fall(cx, cy, score), switch_state(sw), razer_position_x(razer_x) {}
//
//	void fallStep() override {
//		if (position_y == 1) {
//			do {
//				position_x = rand() % 19 + 2;
//			} while (position_x == razer_position_x || position_x % 2 != 0);
//		}
//
//		updatePosition();
//		GREEN printf("¢¾");
//
//		if (position_y == 19) {
//			resetPosition();
//			game_score += 5;
//			switch_state = 0;
//		}
//	}
//};
