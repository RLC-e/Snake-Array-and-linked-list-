#include "Snake.h"

int main()
{
    hideCursor();   //隐藏光标
    srand((unsigned)time(NULL));    //随机时间种子

	init_data_UI();		//初始化数据 初始化图形界面（蛇，食物，墙）
    initgame();         //初始化游戏

    game();             //游戏开始
    gameover();         //游戏结束
}

void init_data_UI(void)
{
    initsnake();    //初始化蛇
    initfood();     //初始化食物
    initwall();     //初始化墙
}

void initsnake(void)
{
    COORD coord;

    //蛇头
    snake = malloc(sizeof(struct Snake));
    if (snake == NULL)
        exit(EXIT_FAILURE);
    first = snake;
    head = snake;

    snake->body.x = WIDE / 2;
    snake->body.y = HIGH / 2;
    snake->next = NULL;

    coord.X = snake->body.x;
    coord.Y = snake->body.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("@");

    //蛇身
    snake = malloc(sizeof(struct Snake));
    if (snake == NULL)
        exit(EXIT_FAILURE);

    snake->body.x = WIDE / 2 - 1;
    snake->body.y = HIGH / 2;
    snake->next = first;
    first = snake;

    coord.X = snake->body.x;
    coord.Y = snake->body.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("*");
}

void initfood(void)
{
    COORD coord;

    //防止生成在蛇上
    do
    {
        food.x = rand() % WIDE;
        food.y = rand() % HIGH;
    } while ((food.x == WIDE / 2 - 1 && food.y == HIGH / 2) || (food.x == WIDE / 2 && food.y == HIGH / 2));

    coord.X = food.x;
    coord.Y = food.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("#");
}

void initwall(void)
{
    COORD coord;

    //下面的墙
    for (int i = 0;i <= WIDE;i++)
    {
        coord.X = i;
        coord.Y = HIGH;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("&");
    }

    //右边的墙
    for (int i = 0;i <= HIGH;i++)
    {
        coord.X = WIDE;
        coord.Y = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("&");
    }
}

void initgame(void)
{
    key = _getch();
    switch (key)
    {
    case 'w':head->body.y--;dir = 'w';break;
    case 's':head->body.y++;dir = 's';break;
    case 'a':head->body.x--;dir = 'a';break;
    case 'd':head->body.x++;dir = 'd';break;
    }
}

void game(void)
{
    while (1)
    {
        collision();    //碰撞判断
        move();         //移动
        draw();         //画面绘制
    }
}

void collision(void)
{
    hit_wall();         //撞墙
    hit_itself();       //撞身体
    eat_food();         //撞食物
}

void hit_wall(void)
{
    if (head->body.x < 0 || head->body.x >= WIDE || head->body.y < 0 || head->body.y >= HIGH)
        exit(EXIT_FAILURE);
}

void hit_itself(void)
{
    for (cur = first;cur->next != NULL;cur = cur->next)
    {
        if (head->body.x == cur->body.x && head->body.y == cur->body.y)
            exit(EXIT_FAILURE);
    }
}

void eat_food(void)
{
    bool a;

    if (food.x == head->body.x && food.y == head->body.y)
    {
        //防止食物生成在蛇上
        do
        {
            a = false;
            food.x = rand() % WIDE;
            food.y = rand() % HIGH;
            for (cur = first;cur != NULL;cur = cur->next)
            {
                if (cur->body.x == food.x && cur->body.y == food.y)
                    a = true;
            }
        } while (a);

        snake = malloc(sizeof(struct Snake));
        if (snake == NULL)
            exit(EXIT_FAILURE);

        snake->next = first;
        first = snake;
    }
}

void move(void)
{
    //蛇身移动
    for (prev = first, cur = first->next;prev->next != NULL;cur = cur->next, prev = prev->next)
    {
        prev->body.x = cur->body.x;
        prev->body.y = cur->body.y;
    }

    //蛇头移动
    if (_kbhit())
    {
        key = _getch();

        Reverse_direction_check();
        switch (key)
        {
        case 'w':head->body.y--;dir = 'w';break;
        case 's':head->body.y++;dir = 's';break;
        case 'a':head->body.x--;dir = 'a';break;
        case 'd':head->body.x++;dir = 'd';break;
        }
    }
    else
        switch (dir)
        {
        case 'w':head->body.y--;break;
        case 's':head->body.y++;break;
        case 'a':head->body.x--;break;
        case 'd':head->body.x++;break;
        }
}

void Reverse_direction_check(void)
{
    if (key == 'w' && dir == 's')
        exit(EXIT_FAILURE);
    if (key == 's' && dir == 'w')
        exit(EXIT_FAILURE);
    if (key == 'a' && dir == 'd')
        exit(EXIT_FAILURE);
    if (key == 'd' && dir == 'a')
        exit(EXIT_FAILURE);
}

void draw(void)
{
    COORD coord;
    system("cls");

    //蛇身
    for (cur = first;cur->next != NULL;cur = cur->next)
    {
        coord.X = cur->body.x;
        coord.Y = cur->body.y;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("*");
    }
    //蛇头
    coord.X = head->body.x;
    coord.Y = head->body.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("@");

    //食物
    coord.X = food.x;
    coord.Y = food.y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    printf("#");

    //墙
    for (int i = 0;i <= WIDE;i++)
    {
        coord.X = i;
        coord.Y = HIGH;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("&");
    }
    //右边的墙
    for (int i = 0;i <= HIGH;i++)
    {
        coord.X = WIDE;
        coord.Y = i;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
        printf("&");
    }

    Sleep(200);
}

void gameover(void)
{
    COORD coord;

    //结束时的文字在场景外
    coord.X = 0;
    coord.Y = HIGH + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}