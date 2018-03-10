#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#define DELAY 200000

void initialize_board();
void alloc_board();
void setup();
void draw_board();
void update();
int has_neighbour_at(int y, int x);
void set_cell(int y, int x);
int count_neighbours(int y, int x);

#endif /* GAME_OF_LIFE_H */
