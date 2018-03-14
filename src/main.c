#include "game_of_life.h"
#include <stdio.h>
#include <string.h>

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("------------------------------------------------\n");
		printf("USAGE:\n");
		printf("\n");
		printf("\tRandom initial state:\n");
		printf("\t\t./main -r\n");
		printf("\n");
		printf("\tDrawing your initial state:\n");
		printf("\t\t./main -i\n");
		printf("\n");
		printf("\tSee this text again:\n");
		printf("\t\t./main\n");
		printf("\n");
		printf("------------------------------------------------\n");
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
