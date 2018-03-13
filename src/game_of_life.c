#include "game_of_life.h"
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <locale.h>

int max_x;
int max_y;
int x = 0;
int y = 0;
int next_x = 0;
int direction = 1;
int** board;
int** next_frame;
int offsets[8][2] = {
	{-1, -1}, {-1, 0}, {-1, +1},
	{ 0, -1},          { 0, +1},
	{+1, -1}, {+1, 0}, {+1, +1} };

static void clear_next_frames()
{
	int i;
	int j;
	for (i = 0; i < max_y; i++)
	{
		for (j = 0; j < max_x; j++)
		{
			next_frame[i][j] = 0;
		}
	}
}

static void initialize_board()
{
	int i;
	int j;
	for (i = 0; i < max_y; i++)
	{
		for (j = 0; j < max_x; j++)
		{
			if ((rand() % 15) == 0)
			{
				board[i][j] = 1;
			}
		}
	}
}

static void alloc_board()
{
	board = (int**) calloc(max_y, sizeof(int*));
	next_frame = (int**) calloc(max_y, sizeof(int*));
	int i;
	for (i = 0; i < max_y; i++)
	{
		board[i] = (int*) calloc(max_x, sizeof(int));
		next_frame[i] = (int*) calloc(max_x, sizeof(int));
	}
}

void dealloc_board()
{
	int i;
	for (i = 0; i < max_y; i++)
	{
		free(board[i]);
		free(next_frame[i]);
	}
	free(board);
	free(next_frame);
}

void setup()
{
	setlocale(LC_ALL, "en_US.UTF-8");
	srand(time(NULL));
	initscr();
	noecho();
	/*cbreak();*/
	nodelay(stdscr, TRUE);
	curs_set(FALSE);
	getmaxyx(stdscr, max_y, max_x);
	alloc_board();
	initialize_board();
}

static int has_neighbour_at(int y, int x)
{
	if (y < 0 || x < 0 || y >= max_y || x >= max_x )
	{
		return 0;
	}
	return board[y][x];
}

static int count_neighbours(int y, int x)
{
	int count = 0;
	int i;
	for (i = 0; i < 8; i++)
	{
		if (has_neighbour_at(y + offsets[i][0], x + offsets[i][1]))
		{
			count++;
		}
	}
	return count;
}

static void draw_board()
{
	int i;
	int j;
	for (i = 0; i < max_y; i++)
	{
		for (j = 0; j < max_x; j++)
		{
			if (board[i][j])
			{
				mvprintw(i, j, "\u2588");
			}
		}
	}
}

void apply_rules()
{
	int i;
	int j;
	int living;
	int count;
	for (i = 0; i < max_y; i++)
	{
		for (j = 0; j < max_x; j++)
		{
			living = board[i][j];
			count = count_neighbours(i, j);
			if (living && count < 2)
			{
				next_frame[i][j] = 0;
			}
			if (living && (count == 2 || count == 3))
			{
				next_frame[i][j] = 1;
			}
			if (living && count > 3)
			{
				next_frame[i][j] = 0;
			}
			if (!living && count == 3)
			{
				next_frame[i][j] = 1;
			}
		}
	}
	int** aux;
	aux = board;
	board = next_frame;
	next_frame = aux;
	clear_next_frames();
}

void update()
{
	while(1)
	{
		clear();
		draw_board();
		refresh();
		usleep(DELAY);
		apply_rules();
		if ('q' == getch())
		{
			dealloc_board();
			endwin();
			break;
		}
	}
}
