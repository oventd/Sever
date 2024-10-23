#include "header.h"

#include "Player.cpp"



class Game {
private:
	int currentTime;
	int kb1, n[20], star_x[20], swit[19], sleep_num;
	int razer_x = 1, razer_y, razer_switch = 0, razer_index = 0;
	int heart_x = 1, heart_y, heart_switch = 0;
	int star_count = 0;
	int death_time = -5;
	Render render;

	struct game_source game_source1;
	//Fall* fallObjects;

public:
	int map[2][20][12]=
	{ {
			1,1,1,1,1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1,1,1,1,1
	},

	{
			1,1,1,1,1,1,1,1,1,1,1,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,2,0,0,0,0,0,0,0,1,
			1,0,2,0,2,0,2,2,2,0,2,1,
			1,0,0,0,0,0,0,0,2,0,2,1,
			1,2,2,2,2,2,2,2,2,0,2,1,
			1,0,0,0,0,0,2,0,0,0,2,1,
			1,0,0,2,0,0,2,2,2,0,2,1,
			1,0,2,0,2,0,0,0,0,0,2,1,
			1,0,0,2,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,0,0,0,0,0,0,0,0,0,0,1,
			1,1,1,1,1,1,1,1,1,1,1,1
	} };
	struct cursor cursor1;

	Game() : cursor1{ 17, 17, 12, 18 }, game_source1{ 0, 1000, 0, 5, 0, 0, 0 }, razer_x(1), heart_x(1), death_time(-5) {}

	void gotoxy(int x, int y);

	void map_ptf(int k);

	void razer(int* razer_x, int* razer_y, int* razer_switch, int* razer_index, int* cursor1cx, int* cursor1cy, int* game_source1score);

	void star(int* n, int* star_x, int* razer_y, int* heart_y, int* cursor1cx, int* cursor1cy, int* game_source1score);

	void heart(int* heart_x, int* heart_y, int* razer_y, int* heart_switch, int* cursor1cx, int* cursor1cy, int* game_source1score);

	void menu();

	void move_menu(int* cursor1by, int* game_source1menu_change, int* game_source1score, int* game_source1score_ptf, int* game_source1begin_menu, int* game_source1diff);

	void Menu();

	void printSIdeInfo();

	void win();
	void lose();

	void updateGameSideInfo(Player player) {
		gotoxy(35, 6);

		LIGHT_GREEN printf("%-4d", game_source1.score);
		gotoxy(35, 8);

		RED printf("%d", player.getLife());
		gotoxy(48, 26);
	}

	void checkPlayerLife(Player player) {
		if (death_time + 15 < currentTime) {
			////heart hit
			//if (player.isHit(heart_y, heart_x) == 1){
			//	player.heal();
			//	death_time = currentTime - 20;
			//	return;
			//}

			////razer hit
			int player_death_status_star = 0;
			//if (player.isHit(razer_y, razer_x) == 1) {
			//	player_death_status_star++;
			//}
			for (int j = 0; j < 20; j++){
				if (player_death_status_star != 0)
					break;

				player_death_status_star = player.isHit(star_x[j], n[j]);

			}
			if (player_death_status_star == 1){
				if (player.getLife() == 0){
					return;
				}

				else{
					player.damage();
					game_source1.score -= 5;
					death_time = currentTime;
					return;
				}
			}
		}
	}

	void initialize() {
		razer_x = 1; razer_y; razer_switch = 0; razer_index = 0;
		heart_x = 1; heart_y; heart_switch = 0;
		star_count = 0;
		death_time = -5;
		cursor1.by = 17, cursor1.cx = 12, cursor1.cy = 18, game_source1.score = 0, game_source1.start_re = 0, game_source1.begin_menu = 0;
		for (int i = 0; i < 20; i++)
			n[i] = 1;
		for (int i = 0; i < 19; i++)
			swit[i] = 0;

		do
		{
			razer_y = rand() % 19 + 2;
		} while ((razer_y % 2 == 0 ? 1 : 0) == 0);//È¦¼ö°¡ ³ª¿À¸é ³×¸ð Å×µÎ¸®Ä­ÀÌ¶û ¶ËÄ­ÀÌ¶û ¸ÂÁú ¾ÊÀ¸´Ï Â¦¼ö·Î ¼³Á¤


		/*ÃÖÃÊ ·£´ý  ÃÊ±âÈ­*/

		for (int i = 0; i < 20; i++){
			do{
				star_x[i] = rand() % 19 + 2;

			} while ((star_x[i] % 2 == 0 ? 1 : 0) == 0);//È¦¼ö°¡ ³ª¿À¸é ³×¸ð Å×µÎ¸®Ä­ÀÌ¶û ¶ËÄ­ÀÌ¶û ¸ÂÁú ¾ÊÀ¸´Ï Â¦¼ö·Î ¼³Á¤ 
		}
	}
	void difficultSet() {
		if (game_source1.diff == 1) {
			sleep_num = 40;
			strcpy(game_source1.diff_ch, "Áß±Þ");
		}
		else if (game_source1.diff == 2) {
			sleep_num = 20;
			strcpy(game_source1.diff_ch, "°í±Þ");
		}
		else {
			sleep_num = 60;
			strcpy(game_source1.diff_ch, "ÃÊ±Þ");
		}
	}


	void run() {
		system("title º° ÇÇÇÏ±â");
		system("mode con: cols=50 lines=27");

		srand(time(NULL));

		while (1){
			initialize();
			
			Menu();
			
			Player player1(16, 18, "¿Ê", 0x0009, 5);
			Player player2(8, 18, "¿Ê", 0x000c, 5);

			difficultSet();

			gotoxy(0, 0);
			map_ptf(0);//¸ÊÃâ·Â 

			printSIdeInfo();


			/*°ÔÀÓ ½ÇÇàÁß*/
			for (currentTime = 1; ; currentTime++)
			{
				Sleep(sleep_num);

				if (_kbhit())
					player1.move();

				if (currentTime % 100 == 0)
				{
					star_count++;
				}

				for (int j = 0; j <= star_count; j++)
				{
					star(&n[j], &star_x[j], &razer_y, &heart_y, &cursor1.cx, &cursor1.cy, &game_source1.score);
				}

				//if (currentTime % 100 == 0) {
				//	razer_switch = 1;
				//}

				//if (razer_switch == 1) {
				//	if (currentTime % 5 == 0) {
				//		razer_index++;
				//	}
				//	razer(&razer_x, &razer_y, &razer_switch, &razer_index, &cursor1.cx, &cursor1.cy, &game_source1.score);
				//}

				//if (rand() % 300 == 0) {
				//	heart_switch = 1;
				//}

				//if (heart_switch == 1) {
				//	heart(&heart_x, &heart_y, &razer_y, &heart_switch, &cursor1.cx, &cursor1.cy, &game_source1.score);
				//}



				
				player2.render();

				checkPlayerLife(player1);

				updateGameSideInfo(player1);

				//printf("%d", player1.getLife());

				if (game_source1.score > game_source1.score_ptf)//½Â¸®Á¶°Ç 
					break;

				else if (player1.getLife() == 0)//ÆÐ¹èÁ¶°Ç 
					break;

			}

			if (game_source1.score > game_source1.score_ptf)
				win();

			else if (player1.getLife() == 0 && game_source1.score < game_source1.score_ptf)
				lose();
		}

		printf("\n\n");
	}

};

