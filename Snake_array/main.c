#include <stdio.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>

//创建场景
#define HIGH 20		
#define WIDE 60

//坐标
struct position	    
{
	int x;
	int y;
};

//定义蛇对象
struct snake		
{
	struct position body[(HIGH - 1) * (WIDE - 1)];
	int size;
}snake;

//定义食物对象
struct food			
{
	struct position create;
}food;

void initsnake(void);
void initfood(void);
void initUI(void);
void snake_move(struct snake *snake);
void hide_cursor(void);
void game(void);
void wall(void);
void initgame(void);
void print_score(int score);
void print_lever(int lever);

int main()
{
	hide_cursor();	//隐藏光标
	srand((unsigned)time(NULL));	//随机时间种子

	initsnake();	//初始化蛇
	initfood();		//初始化食物
	initUI();		//初始化图形
	wall();			//初始化画墙

	initgame();		//初始化游戏
	game();			//游戏开始

}

void initsnake(void)
{
	snake.body[0].x = WIDE / 2;
	snake.body[0].y = HIGH / 2;
}

void initfood(void)
{
	bool a;		//食物是否生成在蛇上
	do
	{
		a = false;
		food.create.x = rand() % (WIDE - 1);
		food.create.y = rand() % (HIGH - 1);

		for (int i = 0;i < 2;i++)
		{
			if (snake.body[i].x == food.create.x && snake.body[i].y == food.create.y)
				a = true;
		}
	} while (a);
}

void initUI(void)
{
	COORD coord;	//声明一个坐标变量

	//蛇头
	coord.X = snake.body[0].x;	
	coord.Y = snake.body[0].y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);	//把控制台的光标，移到 coord 这个坐标
	printf("@");

	//蛇身
	coord.X = snake.body[0].x - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("*");

	//食物
	coord.X = food.create.x;	
	coord.Y = food.create.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("#");
}

void game(void)
{
	snake.size = 2;
	char dir = 'd';	//不控制时自己移动的变量
	char key = 'd';
	int score = 0;
	int sleep = 200;
	int lever = 1;
	COORD coord;
		
	while (1)
	{
		if (_kbhit())		
		{
			key = _getch();
			snake_move(&snake);
			wall();			//画墙
			print_score(score);		//显示分数
			print_lever(lever);		//显示难度

			//回头判定
			if (key == 'w' && dir == 's')
			{
				coord.Y = HIGH + 2;		//光标移动到场景外
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				return 0;
			}
			if (key == 's' && dir == 'w')
			{
				coord.Y = HIGH + 2;		//光标移动到场景外
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				return 0;
			}
			if (key == 'a' && dir == 'd')
			{
				coord.Y = HIGH + 2;		//光标移动到场景外
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				return 0;
			}
			if (key == 'd' && dir == 'a')
			{
				coord.Y = HIGH + 2;		//光标移动到场景外
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				return 0;
			}

			switch (key)	//控制移动
			{
			case 'w':snake.body[0].y -= 1;dir = 'w';break;
			case 's':snake.body[0].y += 1;dir = 's';break;
			case 'a':snake.body[0].x -= 1;dir = 'a';break;
			case 'd':snake.body[0].x += 1;dir = 'd';break;
			}

			coord.X = food.create.x;
			coord.Y = food.create.y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			printf("#");

			coord.X = snake.body[0].x;
			coord.Y = snake.body[0].y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			printf("@");
		}
		else
		{
			snake_move(&snake);		//自己移动	
			wall();				
			print_score(score);
			print_lever(lever);
			switch (dir)
			{
			case 'w':snake.body[0].y -= 1;break;
			case 's':snake.body[0].y += 1;break;
			case 'a':snake.body[0].x -= 1;break;
			case 'd':snake.body[0].x += 1;break;
			}

			coord.X = food.create.x;
			coord.Y = food.create.y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			printf("#");

			coord.X = snake.body[0].x;
			coord.Y = snake.body[0].y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			printf("@");
		}

		if (snake.body[0].x == food.create.x && snake.body[0].y == food.create.y)	//吃食物
		{
			bool a;		//食物是否生成在蛇上
			do		
			{
				a = false;
				food.create.x = rand() % (WIDE - 1);
				food.create.y = rand() % (HIGH - 1);
				coord.X = food.create.x;
				coord.Y = food.create.y;

				for (int i = 0;i < snake.size;i++)
				{
					if (snake.body[i].x == food.create.x && snake.body[i].y == food.create.y)
						a = true;
				}
			} while (a);
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			printf("#");

			score++;
			snake.size++;
		}

		//撞墙判定
		if (snake.body[0].x >= WIDE - 1 || snake.body[0].y >= HIGH - 1 || snake.body[0].x < 0 || snake.body[0].y < 0)
		{
			coord.Y = HIGH + 2;		//光标移动到场景外
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			return 0;
		}

		//撞蛇身判定
		for (int i = snake.size - 1;i > 0;i--)
		{
			if (snake.body[0].x == snake.body[i].x && snake.body[0].y == snake.body[i].y)
			{
				coord.Y = HIGH + 2;		//光标移动到场景外
				SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
				return 0;
			}
		}

		if(score <= 18)
		{
			switch (score)
			{
			case 2: lever = 2;sleep = 180;break;
			case 4: lever = 3;sleep = 160;break;
			case 6: lever = 4;sleep = 140;break;
			case 8: lever = 5;sleep = 120;break;
			case 10: lever = 6;sleep = 100;break;
			case 12: lever = 7;sleep = 80;break;
			case 14: lever = 8;sleep = 60;break;
			case 16: lever = 9;sleep = 40;break;
			case 18: lever = 10;sleep = 20;break;
			}

		}
		Sleep(sleep);	
	}
}

void initgame(void)
{
	COORD coord;

	//按任意键开始游戏
	coord.X = WIDE + WIDE / 4;
	coord.Y = HIGH / 4;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf(">Enert the any key to begin<");

	coord.X = WIDE + WIDE / 3;
	coord.Y = HIGH / 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("Score: 0");

	coord.X = WIDE + WIDE / 3;
	coord.Y = HIGH ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("Lever: 1");

	while (!_kbhit());
}

void snake_move(struct snake *snake)
{
	COORD coord;
	system("cls");
	for (int i = snake->size - 1;i > 0;i--)		//前一节蛇身移动到后一节蛇身
	{
		snake->body[i] = snake->body[i - 1];
		coord.X = snake->body[i].x;
		coord.Y = snake->body[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		printf("*");
	}
}

void hide_cursor()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;  // FALSE = 隐藏，TRUE = 显示
	SetConsoleCursorInfo(console, &info);
}

void wall(void)
{
	COORD coord;
	for (int i = 0;i < WIDE;i++)
	{
		coord.X = i;
		coord.Y = HIGH - 1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		printf("&");
	}
	for (int i = 0;i < HIGH;i++)
	{
		coord.X = WIDE - 1;
		coord.Y = i;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
		printf("&");
	}
}

void print_score(int score)
{
	COORD coord;

	coord.X = WIDE + WIDE / 3;
	coord.Y = HIGH / 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("Score: %d",score);
}

void print_lever(int lever)
{
	COORD coord;

	coord.X = WIDE + WIDE / 3;
	coord.Y = HIGH ;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	printf("Lever: %d", lever);
}