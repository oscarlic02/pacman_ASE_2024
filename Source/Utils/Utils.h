#ifndef  __Utils_h
#define  __Utils_h
#include "../GLCD/GLCD.h"
#include "stdbool.h"

/* Configurations of the maze and display */
#define BOARD_WIDTH 28
#define BOARD_HEIGHT 31
#define TILE_SIZE 8
#define OFFSET 50

/*Values of the matrix */
#define EMPTY 0
#define WALL 1
#define REGULAR_PILL 2
#define PACMAN_START  3
#define PORTAL_LEFT  4
#define PORTAL_RIGHT  8
#define POWER_PILL 50
#define BLINKY_GHOST 5

typedef enum {
    COLOR_WALL = Blue,
    COLOR_REGULAR_PILL = Green,
    COLOR_POWER_PILL = Red,
    COLOR_TEXT = White,
    COLOR_BACKGROUND = Black
} GameColors;

typedef enum {
    RESUME = 0,
    PAUSE = 1,
    GAMEOVER = 2,
    VICTORY = 3
} GameState;

void draw_pill(int x, int y);
void draw_power_pill(int x, int y);

#endif