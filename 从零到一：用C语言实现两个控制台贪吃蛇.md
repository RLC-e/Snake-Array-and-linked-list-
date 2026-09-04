# 从零到一：用C语言实现两个控制台贪吃蛇

#### | 一个准大一新生的第一个完整项目

---

## 一、项目起源

我对独立游戏制作有极高的热情，所以也对代码产生浓厚的兴趣，因此在2026年6月27日开始学习C语言，直到2026年8月25日才完全掌握C语言，为了检测我的学习成果，我选择复刻经典小游戏贪吃蛇

我花了2天的时间，第一天用的是数组贪吃蛇，跟着视频教程做的，第二天用的是链表贪吃蛇，是自己独立思考的，这篇文章记录我从零到一做贪吃蛇的全过程

其中用deepseek补充了**输入检测**，**清屏**，**速度控制**，**光标**等等知识，遇到卡壳实在想不出来就针对性问deepseek并理解其代码

---

## 二、技术选型

###### 项目                                                		第一版												第二版

---

语言														 C语言												 C语言

---

蛇身储存												   数组												   链表

---

输入检测							      	`kbhit()` `getch()	`			     		`kbhit()``getch()`

---

控制台清屏					        		`system("cls")	`								`system("cls")`

---

速度控制					    	       			`	sleep（）`					  	       		`		sleep（）`

---

开发环境           						  	       `	    VS2026		`				  	           			`VS2026`

---

### 关于数组贪吃蛇与链表贪吃蛇的思考

第一版贪吃蛇跟着b站up写的，用一个**结构体里面包含数组**限制蛇的最大长度

```c
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
```

第二版贪吃蛇自己用链表写的，用于提升自己的代码能力

```c
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
```

```c
//蛇头
snake = malloc(sizeof(struct Snake));
if (snake == NULL)
    exit(EXIT_FAILURE);
first = snake;
head = snake;

snake->body.x = WIDE / 2;
snake->body.y = HIGH / 2;
snake->next = NULL;
```

```C
//蛇身
snake = malloc(sizeof(struct Snake));
if (snake == NULL)
    exit(EXIT_FAILURE);

snake->body.x = WIDE / 2 - 1;
snake->body.y = HIGH / 2;
snake->next = first;
first = snake;
```

代码虽然很长很复杂，但是至少我理解了**动态内存分配**

---

## 三、核心功能实现

#### 1.初始化场景

利用`hideCursor()`函数来**隐藏光标**，再利用将光标直接移动到屏幕位置打印蛇，食物，墙来完成**初始化**

因为第一版写的比较乱，所以我用第二版的代码

我声明了一个函数**`init_data_UI（）`**来实现场景初始化

```c
void init_data_UI(void)
{
    initsnake();    //初始化蛇
    initfood();     //初始化食物
    initwall();     //初始化墙
}
```

里面包含初始化蛇，食物，墙，下面先看蛇的

蛇包含**蛇头 蛇身**

```C
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
```

**动态分配**一个 `struct Snake` 节点，`snake` 指针指向它，然后将光标移到中间打印蛇

![屏幕截图 2026-09-03 161124](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-03 161124.png)

再用**随机数**生成食物，这边我用了一个**循环**防止食物是否生成在蛇上，在此之前需要用`srand((unsigned)time(NULL))`**生成随机时间种子**

```C
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
```

![屏幕截图 2026-09-03 161053](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-03 161053.png)

最后只生成下面和右边的墙

```C
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
```

![屏幕截图 2026-09-03 161247](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-03 161247.png)

---

### 2.游戏主循环

游戏主循环有**碰撞检测**，**蛇移动**，**画面绘制**

```C
void game(void)
{
    while (1)
    {
        collision();    //碰撞判断
        move();         //移动
        draw();         //画面绘制
    }
}
```

- **移动逻辑**

  声明函数`move()`

  先让**蛇身**移动到上一节蛇身的位置，然后再移动**蛇头**

  其中我用`kbhit()`检测**是否有按键**，用`getch()`**读取按键值**，然后改变蛇头坐标，并且记录**没有按下按键**时，蛇头朝向的方向

```c
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
```

其中 `Reverse_direction_check()`函数用来**检测回头**（不允许回头）

```C
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
```

- **碰撞判断**

  吃食物，撞到自己，撞到墙我都归为碰撞，于是声明了函数`collision()`

  ```C
  void collision(void)
  {
      hit_wall();         //撞墙
      hit_itself();       //撞身体
      eat_food();         //撞食物
  }
  ```

撞墙判断极为简单，判断是否**超出边界**即可

```C
void hit_wall(void)
{
    if (head->body.x < 0 || head->body.x >= WIDE || head->body.y < 0 || head->body.y >= HIGH)
        exit(EXIT_FAILURE);
}
```

撞自己用**遍历链表**，检测是否和蛇头坐标重合

```C
void hit_itself(void)
{
    for (cur = first;cur->next != NULL;cur = cur->next)
    {
        if (head->body.x == cur->body.x && head->body.y == cur->body.y)
            exit(EXIT_FAILURE);
    }
}
```

撞食物（吃食物）需要**重新生成食物**，然后把**自身长度变长**

采用和**初始化食物**一样的方法还有**遍历链表**检测

然后**重新分配**一块新的内存给新的蛇身，让其指向最后一节蛇身，其余蛇身正常继续移动

```c
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
```

- **画面绘制**

  在绘制之前先用`system()`**清屏**后再继续绘制

  在所有对象坐标全部确认后绘制

  最后使用`sleep（）`延时**防止速度过快**

```C
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
```

在第一版我有分数系统，分数越高速度越快

由于过于混乱我就不在此展示，本质就是改变`sleep`的数值

---

### 3.游戏初始化

游戏开始前是**静止不动**的，所以我用`initgame()`**初始化游戏**，**按键按下**的时候才会**开始游戏**

```C
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
```

---

### 4.游戏结束

为了防止结束后控制台文字覆盖，游戏结束时直接把光标固定在下方

```C
void gameover(void)
{
    COORD coord;

    //结束时的文字在场景外
    coord.X = 0;
    coord.Y = HIGH + 1;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
```

![屏幕截图 2026-09-03 170002](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-03 170002.png)

---

## 四、踩坑

（忘记记录错误代码了，只能说个大概）

#### 1.忘记给`kbhit`加括号

当时已经能移动了，但是很奇怪，初始化后的移动会出现**两个头**，当时没给我笑死

![屏幕截图 2026-09-04 121830](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-04 121830.png)

看了半天的移动逻辑感觉写的天衣无缝，整了半天去问ds说`kbhit`是函数，形式参数是void要加（），没给我气死，算是自己瞎了

---

#### 2.吃食物边吃边拉

当时在写吃食物的逻辑，我在**吃食物的函数**里面调用**移动函数**，导致逻辑极其错乱，从而导致吃食物边吃边拉，可以拉一地，笑死我了

![屏幕截图 2026-09-04 124649](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-04 124649.png)

#### 3.吃食物不能增加长度

重写吃食物的逻辑，我是先**移动头**再**移动身子**，逻辑就是让后面的蛇身跟着前一节蛇身走，但是问题就是我蛇头先走的，导致所有的蛇身都在蛇头的位置，萎缩成一团很好笑（如图所示），而且当时我传数据用的不是**指针**，导致生成的蛇身在左上角（下面没有配图）

![屏幕截图 2026-09-04 122725](C:\Users\RLC-e\Pictures\Screenshots\屏幕截图 2026-09-04 122725.png)

---

## 五、总结

通过这个项目学会了如何从零到一做一个游戏，虽然一路上磕磕绊绊，做出来的那一刻是无比开心的，它验证了我这个暑假的学习成果没有白费，而且学会了如何解决问题，感觉自己成长飞快，离自己的游戏梦又进了一步



在前几天就把easyx学了，已经能做图形化界面了，后面我打算再做一个俄罗斯方块，然后就要去学数据结构了，再把C++学了，然后学unity同时学C#，嘻嘻，加油！

---

## 附：GitHub链接

项目已开源，欢迎指正：

[RLC-e/Snake-Array-and-linked-list-](https://github.com/RLC-e/Snake-Array-and-linked-list-/tree/main)

---

*写于  2026年 9月3日下午  and  9月4日中午  | 一个准大一的计科生*





