#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h> 
#include <ctime>
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
