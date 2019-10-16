#include "game_of_life.h"
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <locale.h>

static int max_x;
static int max_y;
static int** board;
static int** next_frame;
static int offsets[8][2] = {
	{-1, -1}, {-1, 0}, {-1, +1},
	{ 0, -1},          { 0, +1},
	{+1, -1}, {+1, 0}, {+1, +1} };

void load_from_file(char* filepath)
{
	FILE* file = fopen ( filepath, "r" );

	if (file != NULL)
	{
		char line[max_x];

		int line_counter = 0;
		while (fgets(line, sizeof(line), file) != NULL && line_counter <= max_y - TMUX_FIX)
		{
			size_t ln = strlen(line) - 1;
			if (line[ln] == '\n')
			{
				line[ln] = '\0';
			}

			int i;
			for (i = 0; i < strlen(line); i++)
			{
				if(line[i] != ' ')
					board[line_counter][i] = 1;
			}
			line_counter++;
		}
		fclose(file);
	}
	else
	{
		perror(filepath);
		exit(1);
	}
}

static void clear_next_frames(void)
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

void initialize_board(void)
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

static void alloc_board(void)
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

static void dealloc_board(void)
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


void setup_simulation(void)
{
	nodelay(stdscr, TRUE);
}

void setup(void)
{
	// display block character
	setlocale(LC_ALL, "en_US.UTF-8");
	srand(time(NULL));

	// curses related settings
	initscr();
	noecho();
	cbreak();
	curs_set(FALSE);
	// curses mouse settings
	// Enables keypad mode. This makes (at least for me) mouse events getting
	// reported as KEY_MOUSE, instead as of random letters.
	keypad(stdscr, TRUE);
	// don't mask any mouse events
	mousemask(BUTTON1_PRESSED | REPORT_MOUSE_POSITION, NULL);
	// makes the terminal report mouse movement events
	printf("\033[?1003h\n");

	// board related settings
	getmaxyx(stdscr, max_y, max_x);
	alloc_board();
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

static void draw_board(void)
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

static void apply_rules(void)
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

static void prepare_to_exit(void)
{
	dealloc_board();
	endwin();
	// Disable mouse movement events, as l = low
	printf("\033[?1003l\n");
}

void drawing_loop(void)
{
	int first_init = 1;
	while (1)
	{
		erase();
		if (first_init)
		{
			mvprintw(0, 0, "Click anywhere on the screen to draw a block.");
			mvprintw(1, 0, "Press enter to start the simulation.");
			mvprintw(2, 0, "Press q to quit.");
			first_init = 0;
		}
		draw_board();
		refresh();
		int c = getch();
		if (c == '\n' || c == 'q')
		{
			break;
		}
		else
		{
			if (c == KEY_MOUSE)
			{
				MEVENT event;
				if (getmouse(&event) == OK)
				{
					board[event.y][event.x] = 1;
				}
			}
		}
	}
}

void update(void)
{
	while (1)
	{
		if (getch() == 'q')
		{
			prepare_to_exit();
			break;
		}
		erase();
		draw_board();
		napms(1000 / FRAMES_PER_SECOND);
		refresh();
		apply_rules();
	}
}
