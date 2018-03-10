#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#define DELAY 3000

extern int max_x;
extern int max_y;
extern int x;
extern int y;
extern int next_x;
extern int direction;
extern int** board;

void initialize_board();
void alloc_board();
void setup();
void draw_board();
void update();

#endif /* GAME_OF_LIFE_H */
