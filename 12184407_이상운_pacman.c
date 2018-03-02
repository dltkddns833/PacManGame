#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <Windows.h>

#define KEY_ESC      0x1B
#define KEY_8      '8'
#define KEY_2      '2'
#define KEY_4      '4'
#define KEY_6      '6'
#define KEY_W      'w'
#define KEY_X      'x'
#define KEY_A      'a'
#define KEY_D      'd'
// direction ����
#define UP         1
#define DOWN      2
#define LEFT      3
#define RIGHT      4
// maze �̷� ũ��
#define MAZE_H      25
#define MAZE_W      26
// tile type Ÿ�� ����
#define WALL      32
#define FOOD      1
#define PILL      2
#define NUM_PILL   4


// position
typedef struct _POS {
	int row;
	int col;
} POS;


// state of pacman
POS pacman;
int direction = RIGHT;
int speed = 100;

void init();               // initialize
void turn(int key);            // turn
void move();               // move pacman
int check();               // check
POS rand_pos();               // random position

void draw_maze();            // draw maze
void draw_pacman();            // draw pacman
void clear_pacman();         // clear pacman
void show_instruction();      // instruction

void goto_pos(POS pos);
void gotoxy(int x, int y);      // move cursor
void showCursor(BOOL bVisible);


// wall         32,   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,   43
char* tile[] = { "  ", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��", "��" };


int maze[MAZE_H][MAZE_W] =
{
	{ 33,38,38,38,38,38,38,41,38,38,38,38,38,38,38,38,38,38,41,38,38,38,38,38,38,34 },
	{ 37, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1,37 },
	{ 37, 1,33,38,38,34, 1,37, 1,33,38,38,38,38,38,38,34, 1,37, 1,33,38,38,34, 1,37 },
	{ 37, 1,35,38,38,36, 1,37, 1,35,38,38,38,38,38,38,36, 1,37, 1,35,38,38,36, 1,37 },
	{ 37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37 },
	{ 35,38,34, 1,33,34, 1,38,38,38,38, 1,33,34, 1,38,38,38,38, 1,33,34, 1,33,38,36 },
	{ 33,38,36, 1,37,37, 1, 1, 1, 1, 1, 1,37,37, 1, 1, 1, 1, 1, 1,37,37, 1,35,38,34 },
	{ 37, 1, 1, 1,37,35,38,34, 1,33,38,38,36,35,38,38,34, 1,33,38,36,37, 1, 1, 1,37 },
	{ 37, 1, 1, 1,35,38,38,36, 1,35,38,38,38,38,38,38,36, 1,35,38,38,36, 1, 1, 1,37 },
	{ 35,38,34, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,33,38,36 },
	{ 32,32,37, 1,33,38,38,34, 1,33,38,38,38,38,38,38,34, 1,33,38,38,34, 1,37,32,32 },
	{ 32,32,37, 1,37,33,38,36, 1,37, 7, 3,15, 3, 3, 3,37, 1,35,38,34,37, 1,37,32,32 },
	{ 32,32,37, 1,37,37, 1, 1, 1,37, 3,11, 3,19, 3, 3,37, 1, 1, 1,37,37, 1,37,32,32 },
	{ 33,38,36, 1,35,36, 1,37, 1,35,38,38,38,38,38,38,36, 1,37, 1,35,36, 1,35,38,34 },
	{ 37, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37, 1, 1, 1, 1, 1, 1,37 },
	{ 35,38,34, 1,38,38,38,42,38,38,38, 1,33,34, 1,38,38,38,42,38,38,38, 1,33,38,36 },
	{ 32,32,37, 1, 1, 1, 1, 1, 1, 1, 1, 1,37,37, 1, 1, 1, 1, 1, 1, 1, 1, 1,37,32,32 },
	{ 32,32,37, 1,33,38,38,34, 1,33,38,38,36,35,38,38,34, 1,33,38,38,34, 1,37,32,32 },
	{ 33,38,36, 1,35,38,38,36, 1,35,38,38,38,38,38,38,36, 1,35,38,38,36, 1,35,38,34 },
	{ 37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37 },
	{ 37, 1,33,38,34, 1,33,41,38,38,38, 1,33,34, 1,38,38,38,41,34, 1,33,38,34, 1,37 },
	{ 37, 1,37,32,37, 1,37,37, 1, 1, 1, 1,37,37, 1, 1, 1, 1,37,37, 1,37,32,37, 1,37 },
	{ 37, 1,35,38,36, 1,35,36, 1,38,38,38,42,42,38,38,38, 1,35,36, 1,35,38,36, 1,37 },
	{ 37, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,37 },
	{ 35,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,38,36 },
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
		printf("%7.1f sec", (clock() - tic) / 1000.0);

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
	int temp = 0;
	// random seed
	srand((int)time(NULL));

	// items
	/*  TODO
	�˾�(PILL)�� NUM_PILL��ŭ �����ؼ�
	����(FOOD)��ſ� ��ġ��Ų��.

	*/
	while (1)
	{
		pos.row = rand() % MAZE_H;  // �˾��� ��ġ �������� ����
		pos.col = rand() % MAZE_W;
		if (maze[pos.row][pos.col] == FOOD) // �˾��� ������ġ�� ���� ��, ���ǹ� ����
		{
			maze[pos.row][pos.col] = PILL; // FOOD�� PILL�� �ٲ۴�.
			temp++; // �˾��� ������ �����ϴ� ����
		}
		if (temp == NUM_PILL) // temp�� NUM_PILL�� ���� ���� ���� ��� �ݺ��� Ż��
			break;
	}
	// place packman 
	/*  TODO
	�Ѹ�(pacman)�� ������ ��ҿ� ��ġ��Ų��.
	����!
	�Ѹ��� �ڽ��� ��ġ�� ������ �ڵ� �����ϹǷ�
	�Ѹ� ��ġ(packman)�� maze���� 0���� ����

	*/
	pos = rand_pos(); // rand_pos() => �Ѹ��� ��ġ���� �Լ�
	maze[pos.row][pos.col] = 0; // �Ѹ��� ��ġ�� FOOD��ġ �̹Ƿ� FOOD�� ���� ��ġ(0)���� ����
	pacman = pos; // �������� pacman�� �������� pos�� ����
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

	switch (key)
	{
	case KEY_4:      // left
	case KEY_A:
		// TODO
		direction = LEFT; // ����ڰ� KEY_4 or A�� �Է½� direction ������ LEFT����
		break;
	case KEY_6:      // right
	case KEY_D:
		// TODO
		direction = RIGHT; // ����ڰ� KEY_6 or D�� �Է½� direction ������ RIGHT����
		break;
	case KEY_2:      // down
	case KEY_X:
		// TODO
		direction = DOWN; // ����ڰ� KEY_2 or X�� �Է½� direction ������ DOWN����
		break;
	case KEY_8:      // up
	case KEY_W:
		// TODO
		direction = UP; // ����ڰ� KEY_8 or W�� �Է½� direction ������ UP����
		break;
	case KEY_ESC:   // quit
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
	/*

	�Ѹ��� �̵��Ϸ��� ������ ��ҿ�  ". ", "  ", "+ "�� ���ڿ��� �־�� �����ϼ� �ִ�.
	���� ������⿡ ���� ���� �� �Ѹ��� ������ ���� ����.
	�Ѹ��� �̵��� �ڸ��� ���� 0�� ����־� "  "�� ����ϰ� �Ѵ�.
	�Ѹ��� �̵� �� ��, �Ѹ��� �迭�� ��ġ �� ���� �� FOOD����.
	*/
	if (direction == UP)
	{
		if (maze[pacman.row - 1][pacman.col] < 3)
		{
			maze[pacman.row - 1][pacman.col] = 0;
			maze[pacman.row][pacman.col] = 0;
			pacman.row--;
			draw_pacman();
			clear_pacman();
		}

	}
	else if (direction == DOWN)
	{
		if (maze[pacman.row + 1][pacman.col] < 3)
		{
			maze[pacman.row + 1][pacman.col] = 0;
			maze[pacman.row][pacman.col] = 0;
			pacman.row++;
			draw_pacman();
			clear_pacman();
		}
	}
	else if (direction == RIGHT)
	{
		if (maze[pacman.row][pacman.col + 1] < 3)
		{
			maze[pacman.row][pacman.col + 1] = 0;
			maze[pacman.row][pacman.col] = 0;
			pacman.col++;
			draw_pacman();
			clear_pacman();
		}

	}
	else if (direction == LEFT)
	{
		if (maze[pacman.row][pacman.col - 1] < 3)
		{
			maze[pacman.row][pacman.col - 1] = 0;
			maze[pacman.row][pacman.col] = 0;
			pacman.col--;
			draw_pacman();
			clear_pacman();
		}
	}
}


// check
int check()
{
	/*  TODO
	�̷�(maze) �ȿ� �����ִ�
	����(FOOD)�� �˾�(PILL)�� ������ return
	���� �Ծ����� 0�� return


	*/
	int a, b;
	int temp = 0;
	for (a = 0; a < MAZE_H; a++) // �ݺ����� ���� maze[0][0]���� maze[MAZE_H][MAZE_W]���� �˻�
	{
		for (b = 0; b < MAZE_W; b++)
		{
			if (maze[a][b] < 3) //  ". ", "  ", "+ " (1 , 0, 2)�� ��� temp�� ���ϸ鼭 ����
				temp += maze[a][b];
		}
	}
	return temp; // ���������� PILL�� FOOD�� ���� �� " " = 0�� �����Ƿ� 0�� return �Ǿ� main�Լ��� �ݺ��� ����
				 // �ݸ� PILL�� FOOD�� �Ѱ��� ���� ��, 0�� �ƴ� ����� return �Ǿ� main�Լ��� �ݺ��� ����
}


// random position
POS rand_pos()
{
	/*  TODO
	maze ���� ��ġ�� �������� �����Ѵ�.
	����!
	FOOD�� �ִ� ��ġ������ ����


	*/
	POS pos;
	int a, b;
	while (1)
	{
		a = rand() % MAZE_H; // ������ ��ġ�� ����
		b = rand() % MAZE_W;
		if (maze[a][b] == FOOD) // �������� ������ ��ġ�� FOOD�� ��� ����
		{
			pos.row = a; // �ش� ��ġ pos�� ����
			pos.col = b;
			break; // �ݺ��� Ż��
		}
	}
	return pos; // �ݺ������� ������ ��ġ�� return
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
	if (direction == RIGHT) // �������� direction�� ������ִ� ���� RIGHT�� �� �Ѹ� ��ġ�� "<"����
	{
		maze[pacman.row][pacman.col] = "< ";
	}
	else if (direction == LEFT) // �������� direction�� ������ִ� ���� LEFT�� �� �Ѹ� ��ġ�� ">"����
	{
		maze[pacman.row][pacman.col] = "> ";
	}
	else if (direction == UP) // �������� direction�� ������ִ� ���� UP�� �� �Ѹ� ��ġ�� "V"����
	{
		maze[pacman.row][pacman.col] = "V ";
	}
	else if (direction == DOWN) // �������� direction�� ������ִ� ���� DOWN�� �� �Ѹ� ��ġ�� "^"����
	{
		maze[pacman.row][pacman.col] = "^ ";

	}
	printf("%s", maze[pacman.row][pacman.col]); // �Ѹ��� ��� ���
}


// draw maze
void draw_maze()
{
	int i, j;
	/*  TODO
	�̷�(maze)�� �׸���.
	FOOD  ". "
	PILL  "+ "
	WALL  tile �̿�


	*/
	int MAZE[MAZE_H][MAZE_W];
	for (i = 0; i < MAZE_H; i++) // maze�� ���� ���� ���� ��Ű�� ���� MAZE�� local���� ���� �� ����
	{
		for (j = 0; j < MAZE_W; j++)
		{
			MAZE[i][j] = maze[i][j];
		}
	}
	for (i = 0; i < MAZE_H; i++)
	{
		for (j = 0; j < MAZE_W; j++)
		{
			if (MAZE[i][j] > WALL) // 33�̻��� ���� ���̹Ƿ� ������ �Ǵ�.
			{
				if (MAZE[i][j] == 33)     // �� ������ �ش� ���׸��� ��ġ�� ����
					MAZE[i][j] = tile[1];
				else if (MAZE[i][j] == 34)
					MAZE[i][j] = tile[2];
				else if (MAZE[i][j] == 35)
					MAZE[i][j] = tile[3];
				else if (MAZE[i][j] == 36)
					MAZE[i][j] = tile[4];
				else if (MAZE[i][j] == 37)
					MAZE[i][j] = tile[5];
				else if (MAZE[i][j] == 38)
					MAZE[i][j] = tile[6];
				else if (MAZE[i][j] == 39)
					MAZE[i][j] = tile[7];
				else if (MAZE[i][j] == 40)
					MAZE[i][j] = tile[8];
				else if (MAZE[i][j] == 41)
					MAZE[i][j] = tile[9];
				else if (MAZE[i][j] == 42)
					MAZE[i][j] = tile[10];
				else if (MAZE[i][j] == 43)
					MAZE[i][j] = tile[11];
			}
			else
			{
				if (MAZE[i][j] == FOOD) // FOOD(1)�� ���� ��ġ�� ". "�� ǥ��
					MAZE[i][j] = ". ";
				else if (MAZE[i][j] == PILL) // PILL(2)�� ��ġ�� "+ "�� ǥ��
					MAZE[i][j] = "+ ";
				else
					MAZE[i][j] = tile[0]; // ������ ���� ��� �ش� ���� ���� ��� "  "�� ǥ��
			}
			printf("%s", MAZE[i][j]); // �׸� ���
		}
		printf("\n");
	}

}


// show instruction
void show_instruction()
{
	gotoxy(54, 1);      puts("Left(4, A), Right(6, D)");
	gotoxy(54, 2);      puts("Down(2, X), Up(8, W)");
	gotoxy(54, 3);      puts("Quit(ESC)");
	gotoxy(54, 5);      puts("Press any key to start...");
}


// move cursor to pos
void goto_pos(POS pos)
{
	gotoxy(pos.col * 2, pos.row); // ����, ���� �׸��� ". " ���Ⱑ ���ԵǾ� �����Ƿ� *2�� ���ش�.
}


// move cursor to (x,y)
// upper left corner is (0,0)
void gotoxy(int x, int y)
{
	COORD Pos = { x, y };
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