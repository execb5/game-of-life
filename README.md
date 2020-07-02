# Game of Life

My take on Conway's game of life. There's still a lot of things to be
done here, but I got lazy.

## Dependencies

The only dependency is the `ncurses` library.

## Compiling

Execute `make` in the project root

### Frames per second

By default it will run in 60 frames per second, but that can be changed
on the [game_of_life.h](./include/game_of_life.h) file. Don't forget to
compile the program again if the value was changed.

## Usage

- Random initial state: `./main -r`
- Drawing your initial state: `./main -i`
- Load from file: `./main -f file/path/here`
- Help: `./main`

