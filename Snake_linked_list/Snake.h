#ifndef SNAKE_H
#define SNAKE_H

#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>

#define WIDE 60
#define HIGH 20

char key;
char dir;

struct position
{
	int x;
	int y;
}food;

struct Snake
{
	struct position body;
	struct Snake* next;
}*snake,*first,*head,*cur,*prev;

void init_data_UI(void);
void initsnake(void);
void initfood(void);
void initwall(void);
void initgame(void);
void game(void);
void collision(void);
void hit_wall(void);
void hit_itself(void);
void Reverse_direction_check(void);
void eat_food(void);
void move(void);
void draw(void);
void gameover(void);
void hideCursor(void);
#endif