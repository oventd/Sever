#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h> 
#include <ctime>
#include "Render.cpp"
#include <cstdint>
#pragma warning(disable:4996)

#pragma once



#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 0xd
#define ESC 0x1b



#define COL				GetStdHandle(STD_OUTPUT_HANDLE)
#define BLACK			SetConsoleTextAttribute(COL, 0x0000); 
#define DARK_BLUE		SetConsoleTextAttribute(COL, 0x0001);
#define GREEN			SetConsoleTextAttribute(COL, 0x0002);
#define BLUE_GREEN		SetConsoleTextAttribute(COL, 0x0003);
#define BLOOD			SetConsoleTextAttribute(COL, 0x0004);
#define PURPLE			SetConsoleTextAttribute(COL, 0x0005);
#define DARK_YELLOW		SetConsoleTextAttribute(COL, 0x0006);
#define WHITE			SetConsoleTextAttribute(COL, 0x0007);
#define GRAY			SetConsoleTextAttribute(COL, 0x0008);
#define BLUE			SetConsoleTextAttribute(COL, 0x0009);
#define LIGHT_GREEN		SetConsoleTextAttribute(COL, 0x000a);
#define LIGHT_BLUE		SetConsoleTextAttribute(COL, 0x000b);
#define RED				SetConsoleTextAttribute(COL, 0x000c);
#define PLUM			SetConsoleTextAttribute(COL, 0x000d);
#define YELLOW			SetConsoleTextAttribute(COL, 0x000e);
#define LIGHT_WHITE		SetConsoleTextAttribute(COL, 0x000f);

struct cursor
{
	int by, by_last;
	int cx, cy;
	uint16_t value1 = 0x0000;
};

struct game_source
{
	int score, score_ptf;
	int diff;
	int begin_menu, menu_change, start_re;
	char diff_ch[5];
};

class Player {
private:
	int x;           // Player's current X position
	int y;           // Player's current Y position
	const char* shape;   // Player's shape for rendering
	const char* hitShape = "※";
	uint16_t color;      // Player's color
	int life;       // Player's remaining lives
	bool isDead;     // Player's dead status
	

public:
	Render *render1;

	Player(int startX, int startY, const char* playerShape, uint16_t playerColor, int startLife)
		: x(startX), y(startY), shape(playerShape), color(playerColor), life(startLife), isDead(false) {
		render1 = new Render();
	}

	// Method to render the player at their current position
	void render() const {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
		render1->printAt(shape, x, y);
	}

	// Move the player based on key input
	void move() {
		render1->setClear(x, y);
		register int direction;
		direction = _getch();

		switch (direction) {
		case LEFT:
			if (x > 0) x -= 2;
			break;
		case RIGHT:
			if (x < 19) x += 2;
			break;
		}
		
	}

	// Check if the player is dead
	bool isHit(int fallingObjX, int fallingObjY) {
		// If the player's position matches the falling object's position, they are dead
		if (x == fallingObjX && y == fallingObjY) {
			life--;
			if (life <= 0) {
				isDead = true;
			}
			return true;
		}
		return false;
	}

	void heal() {
		life++;
		GREEN render1->printAt(shape, x, y);
	}

	void damage() {
		life--;
		RED render1->printAt(hitShape, x, y);
	}

	// Getters
	int getX() const { return x; }
	int getY() const { return y; }
	bool getIsDead() const { return isDead; }
	int getLife() const { return life; }

	// Setters
	void setX(int newX) { x = newX; }
	void setY(int newY) { y = newY; }
	void setLives(int newLives) { life = newLives; }
};


class Fall {
protected:
	int position_x;   // X 좌표
	int position_y;   // Y 좌표 (떨어지는 위치)
	int cursor_x;     // 커서 X 좌표
	int cursor_y;     // 커서 Y 좌표
	int game_score;   // 게임 점수

	const char* shape;

	Render render;

public:
	Fall(int cx, int cy, int score) : cursor_x(cx), cursor_y(cy), game_score(score) {
		position_x = rand() % 19 + 2;
		position_y = 1;
	}

	virtual void fallStep() = 0; // 자식 클래스에서 구현할 함수

	virtual void resetPosition() {
		clearPreviousPosition();
		BLUE_GREEN render.printAt("웃", cursor_x, cursor_y);
		position_y = 1;
	}

	void clearPreviousPosition() {
		if (position_y != 1) {
			render.setClear(position_x, position_y - 1);
		}
	}

	void updatePosition() {
		if (position_y < 19) {
			clearPreviousPosition();
			render.printAt(shape,position_x, position_y);
			position_y++;
		}
		else {
			resetPosition();
		}

	}



	int getPositionX() const { return position_x; }
	int getPositionY() const { return position_y; }
	int& getGameScore() { return game_score; }
};

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
//						RED printf("┃");
//					}
//				}
//			}
//		}
//		else if (index >= 11) {
//			if (position_y < 19) {
//				gotoxy(position_x, position_y);
//				RED printf("■");
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
//		shape = "★";
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
//		GREEN printf("♥");
//
//		if (position_y == 19) {
//			resetPosition();
//			game_score += 5;
//			switch_state = 0;
//		}
//	}
//};


class Game {
private:
	struct cursor cursor1;
	struct game_source game_source1;
	int currentTime;
	int kb1, n[20], ran[20], swit[19], sleep_num;
	int razer_n = 1, razer_ran, razer_switch = 0, razer_index = 0;
	int heart_n = 1, heart_ran, heart_switch = 0;
	int j_count = 0;
	int death_time = -5;
	Render render;
	Fall* fallObjects;

	int map[2][20][12] =
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
public:
	void gotoxy(int x, int y)
	{
		COORD Cur;
		Cur.X = x;
		Cur.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
	}

	void map_ptf(int k)
	{
		int i, j;

		for (i = 0; i < 20; i++)
		{
			for (j = 0; j < 12; j++)
			{
				switch (map[k][i][j])
				{
				case 0:
					printf("  "); break;
				case 1:
					PLUM printf("□"); break;
				case 2:
					YELLOW printf("▒"); break;
				}
			}
			putchar('\n');
		}
	}

	void razer(int* razer_n, int* razer_ran, int* razer_switch, int* razer_index, int* cursor1cx, int* cursor1cy, int* game_source1score)
	{
		if (*razer_index == 1) {
			do
			{
				*razer_ran = rand() % 19 + 2;
			} while (((*razer_ran) % 2 == 0 ? 1 : 0) == 0);
		}

		if (*razer_index > 4 && *razer_index < 11) {
			if (*razer_index % 2 == 0) {
				for (int i = 1; i < 18; i++) {
					gotoxy(*razer_ran, i);

					printf("  ");
				}
			}
			else {
				for (int i = 1; i < 19; i++) {
					if (i % 2 == 1) {
						gotoxy(*razer_ran, i);

						RED printf("┃");
					}
				}
			}
		}

		else if (*razer_index >= 11) {
			if (*razer_n < 19)

			{
				gotoxy(*razer_ran, *razer_n);
				RED printf("■");
				(*razer_n)++;
			}
			if (*razer_n >= 19) {
				gotoxy(*razer_ran, *razer_n - 1);
				RED printf("■");
			}

			if (*razer_index > 24)
			{
				for (int i = 1; i < 19; i++) {
					gotoxy(*razer_ran, i);

					printf("  ");
				}
				gotoxy(*cursor1cx, *cursor1cy);

				BLUE_GREEN printf("%s", "웃");

				*razer_n = 1;

				*game_source1score += 10;

				*razer_switch = 0;
				*razer_index = 0;
				*razer_ran = 0;
			}
			gotoxy(48, 26);
		}





	}

	void star(int* n, int* ran, int* razer_ran, int* heart_ran, int* cursor1cx, int* cursor1cy, int* game_source1score)
	{
		if (*n == 1) {
			int do_switch = 0;
			do
			{
				*ran = rand() % 19 + 2;
				if (*ran != *razer_ran && *ran % 2 == 0) {
					do_switch = 1;
				}
			} while (do_switch == 0);
		}

		if (*n < 19)
		{
			if (*n != 1)
			{
				gotoxy(*ran, *n - 1);
				printf("  ");
			}
			gotoxy(*ran, *n);
			YELLOW printf("★");
			(*n)++;
		}

		else
		{
			gotoxy(*ran, *n - 1);
			printf("  ");

			gotoxy(*cursor1cx, *cursor1cy);

			BLUE_GREEN printf("%s", "웃");

			*n = 1;

			*game_source1score += 5;
		}
		gotoxy(48, 26);
	}

	void heart(int* heart_n, int* heart_ran, int* razer_ran, int* heart_switch, int* cursor1cx, int* cursor1cy, int* game_source1score) //김도훈 기능 함수
	{
		if (*heart_n == 1) {
			int do_switch = 0;
			do
			{
				*heart_ran = rand() % 19 + 2;
				if (*heart_ran != *razer_ran && *heart_ran % 2 == 0) {
					do_switch = 1;
				}
			} while (do_switch == 0);
		}

		if (*heart_n < 19)
		{
			if (*heart_n != 1)
			{
				gotoxy(*heart_ran, *heart_n - 1);
				printf("  ");
			}

			gotoxy(*heart_ran, *heart_n);
			GREEN printf("♥");

			(*heart_n)++;
		}

		else
		{
			gotoxy(*heart_ran, *heart_n - 1);
			printf("  ");

			gotoxy(*cursor1cx, *cursor1cy);
			BLUE_GREEN printf("%s", "웃");
			*heart_n = 1;
			*game_source1score += 5;
			*heart_switch = 0;
		}
		gotoxy(48, 26);
	}


	void move(int* cursor1cx, int* cursor1cy)
	{
		register int kb;
		kb = _getch();
		switch (kb)
		{
		case LEFT:
			if (map[0][*cursor1cy][*cursor1cx / 2 - 1] == 0)//이동할 위치에 배열이 있다.
				//이동할 위치 = 배열값이니 충돌조건따진다. 
			{
				gotoxy(*cursor1cx, *cursor1cy);//현재 캐릭터로 커서이동 
				printf("  ");//지우기 
				*cursor1cx -= 2;//방향키따라 커서 값 변경 
				gotoxy(*cursor1cx, *cursor1cy);//커서 이동 
				BLUE_GREEN printf("%s", "웃");
				gotoxy(48, 26);//커서 내보내기 
			}
			break;

		case RIGHT:
			if (map[0][*cursor1cy][*cursor1cx / 2 + 1] == 0)
			{
				gotoxy(*cursor1cx, *cursor1cy);
				printf("  ");
				*cursor1cx += 2;
				gotoxy(*cursor1cx, *cursor1cy);
				BLUE_GREEN printf("%s", "웃");
				gotoxy(48, 26);
			}
			break;

		}
	}





	void menu()
	{
		LIGHT_GREEN
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

	int player_dead(int* ran, int* n, int heart_index, int* cursor1cx, int* cursor1cy)
	{
		if (*ran == *cursor1cx && *n == 19)
		{
			render.gotoxy(*cursor1cx, *cursor1cy);
			if (heart_index == 1) {
				LIGHT_GREEN printf("\a웃");
			}
			else {
				RED printf("\a※");
			}
			return 1;
		}

		else
			return 0;

	}


	void move_menu(int* cursor1by, int* game_source1menu_change, int* game_source1score, int* game_source1score_ptf, int* game_source1begin_menu, int* game_source1diff)
	{
		int kb1 = _getch();
		WHITE
		switch (kb1)
		{
		case DOWN:
			if (*cursor1by + 3 <= 23){
				render.setClear(16, *cursor1by);
				render.setClear(32, *cursor1by);
				*cursor1by += 3;
			}

			break;

		case UP:
			if (*cursor1by - 3 >= 17){
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

	void Menu() {
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

	void printSIdeInfo() {
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
	void win() {
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
	void lose(){
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

	void updateGameSideInfo(Player player) {
		gotoxy(35, 6);

		LIGHT_GREEN printf("%-4d", game_source1.score);
		gotoxy(35, 8);

		RED printf("%d", player.getLife());
		gotoxy(48, 26);
	}

	void checkPlayerLife(Player player) {
		if (death_time + 15 < currentTime) {
			//heart hit
			if (player.isHit(heart_ran, heart_n) == 1)
			{
				player.heal();
				death_time = currentTime - 20;
				return;
			}

			//razer hit
			int player_death_status_star = 0;
			if (player.isHit(razer_ran, razer_n) == 1) {
				player_death_status_star++;
			}
			for (int j = 0; j < 20; j++)
			{
				if (player_death_status_star != 0)
					break;

				player_death_status_star = player.isHit(ran[j], n[j]);

			}
			if (player_death_status_star == 1)
			{
				if (player.getLife() == 0)
				{
					return;
				}

				else
				{
					player.damage();
					game_source1.score -= 5;
					death_time = currentTime;
					return;
				}
			}
		}
	}
	
	void initialize() {
		razer_n = 1; razer_ran; razer_switch = 0; razer_index = 0;
		heart_n = 1; heart_ran; heart_switch = 0;
		j_count = 0;
		death_time = -5;
		cursor1.by = 17, cursor1.cx = 12, cursor1.cy = 18, game_source1.score = 0,  game_source1.start_re = 0, game_source1.begin_menu = 0;
		for (int i = 0; i < 20; i++)
			n[i] = 1;
		for (int i = 0; i < 19; i++)
			swit[i] = 0;

		do
		{
			razer_ran = rand() % 19 + 2;
		} while ((razer_ran % 2 == 0 ? 1 : 0) == 0);//홀수가 나오면 네모 테두리칸이랑 똥칸이랑 맞질 않으니 짝수로 설정


		/*최초 랜덤  초기화*/

		for (int i = 0; i < 20; i++)
		{
			do
			{
				ran[i] = rand() % 19 + 2;

			} while ((ran[i] % 2 == 0 ? 1 : 0) == 0);//홀수가 나오면 네모 테두리칸이랑 똥칸이랑 맞질 않으니 짝수로 설정 
		}
	}
	void difficultSet() {
		if (game_source1.diff == 0) {
			sleep_num = 60;
			strcpy(game_source1.diff_ch, "초급");
		}
		else if (game_source1.diff == 1) {
			sleep_num = 40;
			strcpy(game_source1.diff_ch, "중급");
		}
		else if (game_source1.diff == 2) {
			sleep_num = 20;
			strcpy(game_source1.diff_ch, "고급");
		}
	}
	void updatePlayer() {
		gotoxy(cursor1.cx, cursor1.cy);
		BLUE_GREEN printf("%s", "웃");
	}

	void run() {
		cursor1 = { 17,17,12,18 };
		game_source1 = { 0,1000,0,5,0,0,0,"초급" };


		system("title 별 피하기");
		system("mode con: cols=50 lines=27");

		srand(time(NULL));

		while (1)
		{		
			Menu();
			initialize();
			Player player1(16, 18, "옷", 0x0009, 5);
			Player player2(8, 18, "옷", 0x000c, 5);

			difficultSet();

			gotoxy(0, 0);
			map_ptf(0);//맵출력 

			updatePlayer();


			printSIdeInfo();


			/*게임 실행중*/
			for (currentTime = 1; ; currentTime++)
			{
				Sleep(sleep_num);

				if (_kbhit())
					player1.move();



				if (currentTime % 100 == 0)
				{
					j_count++;
				}

				if (currentTime % 100 == 0) {
					razer_switch = 1;
				}

				if (razer_switch == 1) {
					if (currentTime % 5 == 0) {
						razer_index++;
					}
					razer(&razer_n, &razer_ran, &razer_switch, &razer_index, &cursor1.cx, &cursor1.cy, &game_source1.score);
				}

				if (rand() % 300 == 0) {
					heart_switch = 1;
				}

				if (heart_switch == 1) {
					heart(&heart_n, &heart_ran, &razer_ran, &heart_switch, &cursor1.cx, &cursor1.cy, &game_source1.score);
				}

				for (int j = 0; j <= j_count; j++)
				{
					star(&n[j], &ran[j], &razer_ran, &heart_ran, &cursor1.cx, &cursor1.cy, &game_source1.score);
				}

				player1.render();
				player2.render();

				checkPlayerLife(player1);

				updateGameSideInfo(player1);

				if (game_source1.score > game_source1.score_ptf)//승리조건 
					break;

				else if (player1.getLife() == 0)//패배조건 
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


int main()
{
	Game game;
	game.run();
	return 0;
}

