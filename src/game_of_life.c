#include "game_of_life.h"
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>

int max_x;
int max_y;
int x = 0;
int y = 0;
int next_x = 0;
int direction = 1;
int** board;

void initialize_board()
{
	int i;
	int j;
	for (i = 0; i < max_y; i++)
	{
		for (j = 0; j < max_x; j++)
		{
			if ((rand() % 10) == 0)
			{
				board[i][j] = 1;
			}
		}
	}
}

void alloc_board()
{
	board = (int**) calloc(max_y, sizeof(int*));
	int i;
	for (i = 0; i < max_y; i++)
	{
		board[i] = (int*) calloc(max_x, sizeof(int));
	}
}

void setup()
{
	srand(time(NULL));
	initscr();
	noecho();
	curs_set(FALSE);
	getmaxyx(stdscr, max_y, max_x);
	alloc_board();
	initialize_board();
}

void draw_board()
{
	int i;
	int j;
	for (i = 0; i < max_y; i++)
	{
		for (j = 0; j < max_x; j++)
		{
			if (board[i][j])
			{
				mvprintw(i, j, "M");
			}
		}
	}
}

void update()
{
	while(1)
	{
		clear();
		mvprintw(y, x, "o");
		draw_board();
		refresh();
		usleep(DELAY);
		getch();
	}
	endwin();
}
