#include "game_of_life.h"

int main(int argc, char* argv[])
{
	setup();
	drawing_loop();
	setup_simulation();
	update();
	return 0;
}
