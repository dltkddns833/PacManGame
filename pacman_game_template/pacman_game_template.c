#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define KEY_ESC		0x1B
#define KEY_8		'8'
#define KEY_2		'2'
#define KEY_4		'4'
#define KEY_6		'6'
#define KEY_W		'w'
#define KEY_X		'x'
#define KEY_A		'a'
#define KEY_D		'd'
// direction ����
#define UP			1
#define DOWN		2
#define LEFT		3
#define RIGHT		4
// maze �̷� ũ��
#define MAZE_H		25
#define MAZE_W		26
// tile type Ÿ�� ����
#define WALL		32
#define FOOD		1
#define PILL		2
#define NUM_PILL	4


// position
typedef struct _POS{
	int row;
	int col;
} POS;


// state of pacman
POS pacman;
int direction = RIGHT;
int speed = 100;

void init();					// initialize
void turn(int key);				// turn
void move();					// move pacman
int check();					// check
POS rand_pos();					// random position

void draw_maze();				// draw maze
void draw_pacman();				// draw pacman
void clear_pacman();			// clear pacman
void show_instruction();		// instruction

void goto_pos(POS pos);
void gotoxy(int x, int y);		// move cursor
void showCursor(BOOL bVisible);


// wall         32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43
char* tile[] = {"  ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��"};


int maze[MAZE_H][MAZE_W] = 
{
	{33,38,38,38,38,38,38,41,38,38,38,38,38,38,38,38,38,38,41,38,38,38,38,38,38,34},
	{37, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1,37},
	{37, 1,33,38,38,34, 1,37, 1,33,38,38,38,38,38,38,34, 1,37, 1,33,38,38,34, 1,37},
	{37, 1,35,38,38,36, 1,37, 1,35,38,38,38,38,38,38,36, 1,37, 1,35,38,38,36, 1,37},
	{37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37},
	{35,38,34, 1,33,34, 1,38,38,38,38, 1,33,34, 1,38,38,38,38, 1,33,34, 1,33,38,36},
	{33,38,36, 1,37,37, 1, 1, 1, 1, 1, 1,37,37, 1, 1, 1, 1, 1, 1,37,37, 1,35,38,34},
	{37, 1, 1, 1,37,35,38,34, 1,33,38,38,36,35,38,38,34, 1,33,38,36,37, 1, 1, 1,37},
	{37, 1, 1, 1,35,38,38,36, 1,35,38,38,38,38,38,38,36, 1,35,38,38,36, 1, 1, 1,37},
	{35,38,34, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,33,38,36},
	{32,32,37, 1,33,38,38,34, 1,33,38,38,38,38,38,38,34, 1,33,38,38,34, 1,37,32,32},
	{32,32,37, 1,37,33,38,36, 1,37, 7, 3,15, 3, 3, 3,37, 1,35,38,34,37, 1,37,32,32},
	{32,32,37, 1,37,37, 1, 1, 1,37, 3,11, 3,19, 3, 3,37, 1, 1, 1,37,37, 1,37,32,32},
	{33,38,36, 1,35,36, 1,37, 1,35,38,38,38,38,38,38,36, 1,37, 1,35,36, 1,35,38,34},
	{37, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1,37},
	{35,38,34, 1,38,38,38,42,38,38,38, 1,33,34, 1,38,38,38,42,38,38,38, 1,33,38,36},
	{32,32,37, 1, 1, 1, 1, 1, 1, 1, 1, 1,37,37, 1, 1, 1, 1, 1, 1, 1, 1, 1,37,32,32},
	{32,32,37, 1,33,38,38,34, 1,33,38,38,36,35,38,38,34, 1,33,38,38,34, 1,37,32,32},
	{33,38,36, 1,35,38,38,36, 1,35,38,38,38,38,38,38,36, 1,35,38,38,36, 1,35,38,34},
	{37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37},
	{37, 1,33,38,34, 1,33,41,38,38,38, 1,33,34, 1,38,38,38,41,34, 1,33,38,34, 1,37},
	{37, 1,37,32,37, 1,37,37, 1, 1, 1, 1,37,37, 1, 1, 1, 1,37,37, 1,37,32,37, 1,37},
	{37, 1,35,38,36, 1,35,36, 1,38,38,38,42,42,38,38,38, 1,35,36, 1,35,38,36, 1,37},
	{37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37},
	{35,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,36},
};


int main(void)
{
	int ch;
	clock_t tic;

	// initialize the game
	init();

	// instruction
	show_instruction();
	turn(_getch());

	// timer start
	tic = clock();

	// play
	while (ch = check())
	{
		if (_kbhit())
			turn(_getch());
		
		clear_pacman();
		move();
		draw_pacman();

		// timer
		gotoxy(54, 8);
		printf("%7.1f sec", (clock()-tic)/1000.0);

		Sleep(speed);
	}

	// exit
	gotoxy(54, 10);
	puts("Done!");
	
	gotoxy(0, MAZE_H);
	return 0;
}


// initialize
void init()
{
	POS pos;
	int i;

	// random seed
	srand((int)time(NULL));

	// items
	/*  TODO
		�˾�(PILL)�� NUM_PILL��ŭ �����ؼ�
		����(FOOD)��ſ� ��ġ��Ų��.

	*/

	// place packman 
	/*  TODO
		�Ѹ�(pacman)�� ������ ��ҿ� ��ġ��Ų��.
		����!
		�Ѹ��� �ڽ��� ��ġ�� ������ �ڵ� �����ϹǷ�
		�Ѹ� ��ġ(packman)�� maze���� 0���� ����

	*/

	// clear screen
	system("cls");

	// init window
	showCursor(FALSE);

	// draw
	draw_maze();
	draw_pacman();
}


// turn the direction of pacman
void turn(int key)
{
	switch(key)
	{
	case KEY_4:		// left
	case KEY_A:
		// TODO

		break;
	case KEY_6:		// right
	case KEY_D:
		// TODO

		break;
	case KEY_2:		// down
	case KEY_X:
		// TODO

		break;
	case KEY_8:		// up
	case KEY_W:
		// TODO

		break;
	case KEY_ESC:	// quit
		gotoxy(54, 10);
		puts("Exit!\n");

		gotoxy(0, MAZE_H);
		exit(0);
		break;
	}
}


// move pacman in the current direction
void move()
{
	/*  TODO
		���� ���� ����(direction)�� ����
		�Ѹ��� �̵����� ��ġ(pacman)�� ����

		���� ��ġ�� �ִ� ������ �ڵ����� �����ϹǷ�
		�Ѹ� ��ġ(packman)�� maze���� 0���� ����

		������⿡ ���� ������ ���ڸ���


	*/
}


// check
int check()
{
	/*  TODO
		�̷�(maze) �ȿ� �����ִ� 
		����(FOOD)�� �˾�(PILL)�� ������ return
		���� �Ծ����� 0�� return


	*/
}


// random position
POS rand_pos()
{
	/*  TODO
		maze ���� ��ġ�� �������� �����Ѵ�.
		����!
		FOOD�� �ִ� ��ġ������ ����


	*/
}


// clear pacman
void clear_pacman()
{
	// move cursor
	goto_pos(pacman);
	printf("  ");
}


// draw pacman
void draw_pacman()
{
	// move cursor
	goto_pos(pacman);

	/*  TODO
		packman�� �׸���.
		����(direction)�� ���� V ^ > < �������

	*/
}


// draw maze
void draw_maze()
{
	/*  TODO
		�̷�(maze)�� �׸���.
		FOOD  ". "
		PILL  "+ "
		WALL  tile �̿�


	*/

}


// show instruction
void show_instruction()
{
	gotoxy(54,1);		puts("Left(4, A), Right(6, D)");
	gotoxy(54,2);		puts("Down(2, X), Up(8, W)");
	gotoxy(54,3);		puts("Quit(ESC)");
	gotoxy(54,5);		puts("Press any key to start...");
}


// move cursor to pos
void goto_pos(POS pos)
{
	gotoxy(pos.col*2, pos.row);
}


// move cursor to (x,y)
// upper left corner is (0,0)
void gotoxy(int x, int y)
{
	COORD Pos = {x, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}


// show cursor
void showCursor(BOOL bVisible)
{
	CONSOLE_CURSOR_INFO CurInfo;
	CurInfo.dwSize = 100;
	CurInfo.bVisible = bVisible;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &CurInfo);
}
