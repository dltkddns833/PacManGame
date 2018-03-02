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
// direction 방향
#define UP         1
#define DOWN      2
#define LEFT      3
#define RIGHT      4
// maze 미로 크기
#define MAZE_H      25
#define MAZE_W      26
// tile type 타일 종류
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
char* tile[] = { "  ", "┌", "┐", "└", "┘", "│", "─", "├", "┤", "┬", "┴", "┼" };


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
	알약(PILL)을 NUM_PILL만큼 생성해서
	음식(FOOD)대신에 위치시킨다.

	*/
	while (1)
	{
		pos.row = rand() % MAZE_H;  // 알약의 위치 랜덤으로 설정
		pos.col = rand() % MAZE_W;
		if (maze[pos.row][pos.col] == FOOD) // 알약이 음식위치에 있을 시, 조건문 실행
		{
			maze[pos.row][pos.col] = PILL; // FOOD를 PILL로 바꾼다.
			temp++; // 알약의 갯수를 저장하는 변수
		}
		if (temp == NUM_PILL) // temp가 NUM_PILL의 갯수 까지 됫을 경우 반복문 탈출
			break;
	}
	// place packman 
	/*  TODO
	팩맨(pacman)을 임의의 장소에 위치시킨다.
	주의!
	팩맨은 자신의 위치의 음식을 자동 섭취하므로
	팩맨 위치(packman)의 maze값은 0으로 변경

	*/
	pos = rand_pos(); // rand_pos() => 팩맨의 위치지정 함수
	maze[pos.row][pos.col] = 0; // 팩맨의 위치는 FOOD위치 이므로 FOOD를 먹은 위치(0)으로 설정
	pacman = pos; // 전역변수 pacman에 지역변수 pos를 대입
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
		direction = LEFT; // 사용자가 KEY_4 or A를 입력시 direction 변수에 LEFT대입
		break;
	case KEY_6:      // right
	case KEY_D:
		// TODO
		direction = RIGHT; // 사용자가 KEY_6 or D를 입력시 direction 변수에 RIGHT대입
		break;
	case KEY_2:      // down
	case KEY_X:
		// TODO
		direction = DOWN; // 사용자가 KEY_2 or X를 입력시 direction 변수에 DOWN대입
		break;
	case KEY_8:      // up
	case KEY_W:
		// TODO
		direction = UP; // 사용자가 KEY_8 or W를 입력시 direction 변수에 UP대입
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
	현재 진행 방향(direction)에 따라
	팩맨을 이동시켜 위치(pacman)를 갱신

	현재 위치에 있는 음식은 자동으로 섭취하므로
	팩맨 위치(packman)의 maze값은 0으로 변경

	진행방향에 벽이 있으면 제자리에


	*/
	/*

	팩맨이 이동하려는 방향의 장소에  ". ", "  ", "+ "의 문자열이 있어야 움직일수 있다.
	따라서 진행방향에 벽이 있을 시 팩맨은 움직일 수가 없다.
	팩맨이 이동한 자리의 값은 0을 집어넣어 "  "를 출력하게 한다.
	팩맨이 이동 할 시, 팩맨의 배열의 위치 값 변경 및 FOOD제거.
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
	미로(maze) 안에 남아있는
	먹이(FOOD)나 알약(PILL)의 개수를 return
	전부 먹었으면 0을 return


	*/
	int a, b;
	int temp = 0;
	for (a = 0; a < MAZE_H; a++) // 반복문을 통해 maze[0][0]부터 maze[MAZE_H][MAZE_W]까지 검사
	{
		for (b = 0; b < MAZE_W; b++)
		{
			if (maze[a][b] < 3) //  ". ", "  ", "+ " (1 , 0, 2)를 모두 temp에 더하면서 저장
				temp += maze[a][b];
		}
	}
	return temp; // 최종적으로 PILL과 FOOD가 없을 시 " " = 0만 남으므로 0이 return 되어 main함수의 반복문 종료
				 // 반면 PILL과 FOOD가 한개라도 있을 시, 0이 아닌 양수가 return 되어 main함수의 반복문 실행
}


// random position
POS rand_pos()
{
	/*  TODO
	maze 안의 위치를 랜덤으로 선택한다.
	주의!
	FOOD가 있는 위치에서만 선택


	*/
	POS pos;
	int a, b;
	while (1)
	{
		a = rand() % MAZE_H; // 랜덤한 위치를 지정
		b = rand() % MAZE_W;
		if (maze[a][b] == FOOD) // 랜덤으로 생성된 위치가 FOOD일 경우 실행
		{
			pos.row = a; // 해당 위치 pos에 저장
			pos.col = b;
			break; // 반복문 탈출
		}
	}
	return pos; // 반복문에서 설정된 위치를 return
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
	packman을 그린다.
	방향(direction)에 따라 V ^ > < 모양으로

	*/
	if (direction == RIGHT) // 전역변수 direction에 저장되있는 값이 RIGHT일 때 팩맨 위치에 "<"대입
	{
		maze[pacman.row][pacman.col] = "< ";
	}
	else if (direction == LEFT) // 전역변수 direction에 저장되있는 값이 LEFT일 때 팩맨 위치에 ">"대입
	{
		maze[pacman.row][pacman.col] = "> ";
	}
	else if (direction == UP) // 전역변수 direction에 저장되있는 값이 UP일 때 팩맨 위치에 "V"대입
	{
		maze[pacman.row][pacman.col] = "V ";
	}
	else if (direction == DOWN) // 전역변수 direction에 저장되있는 값이 DOWN일 때 팩맨 위치에 "^"대입
	{
		maze[pacman.row][pacman.col] = "^ ";

	}
	printf("%s", maze[pacman.row][pacman.col]); // 팩맨의 모양 출력
}


// draw maze
void draw_maze()
{
	int i, j;
	/*  TODO
	미로(maze)를 그린다.
	FOOD  ". "
	PILL  "+ "
	WALL  tile 이용


	*/
	int MAZE[MAZE_H][MAZE_W];
	for (i = 0; i < MAZE_H; i++) // maze의 숫자 값을 유지 시키기 위해 MAZE의 local변수 선언 후 복사
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
			if (MAZE[i][j] > WALL) // 33이상의 값이 벽이므로 조건을 건다.
			{
				if (MAZE[i][j] == 33)     // 각 숫자의 해당 벽그림을 위치에 대입
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
				if (MAZE[i][j] == FOOD) // FOOD(1)의 값의 위치에 ". "를 표시
					MAZE[i][j] = ". ";
				else if (MAZE[i][j] == PILL) // PILL(2)의 위치에 "+ "를 표시
					MAZE[i][j] = "+ ";
				else
					MAZE[i][j] = tile[0]; // 상위의 값에 모두 해당 되지 않을 경우 "  "를 표시
			}
			printf("%s", MAZE[i][j]); // 그림 출력
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
	gotoxy(pos.col * 2, pos.row); // 먹이, 벽의 그림이 ". " 띄어쓰기가 포함되어 있으므로 *2를 해준다.
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