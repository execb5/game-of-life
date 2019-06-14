#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#define DELAY 200000

void setup(void);
void setup_simulation(void);
void update(void);
void drawing_loop(void);
void initialize_board(void);
void load_from_file(char* filepath);

#endif /* GAME_OF_LIFE_H */
