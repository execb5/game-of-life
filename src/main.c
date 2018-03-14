#include "game_of_life.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("");
		return 0;
	}
	setup();
	if (strcmp("-r", argv[1]) == 0)
	{
		initialize_board();
	}
	if (strcmp("-i", argv[1]) == 0)
	{
		drawing_loop();
	}
	setup_simulation();
	update();
	return 0;
}
