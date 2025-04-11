#ifndef  __Pacman_h
#define  __Pacman_h
#include "stdbool.h"
#include "stdint.h"
#include "../GLCD/GLCD.h"

#define BOARD_WIDTH 28
#define BOARD_HEIGHT 31
#define TILE_SIZE 8
#define OFFSET 50
#define MAX_SCORE 2640
#define REGULAR_PILL_SCORE 10
#define POWER_PILL_SCORE 50
#define INITIAL_TIME 60
#define INITIAL_POSITION 1


#define CHASE 0
#define FRIGHTENED 1
#define RESPAWNING 2
#define RESPAWN_TIME 0x000124F8 // 3 milliseconds at 25Mhz timer (K=0.03*25.000.000=75.000 => 0x00124F8)



typedef enum {
    RESUME = 0,
    PAUSE = 1,
    GAMEOVER = 2,
    VICTORY = 3
} GameState;

typedef enum {
    COLOR_WALL = Blue,
    COLOR_REGULAR_PILL = Green,
    COLOR_POWER_PILL = Red,
    COLOR_TEXT = White,
    COLOR_BACKGROUND = Black
} GameColors;

typedef enum {
    UP = 1,
    DOWN = -1,
    RIGHT = 2,
    LEFT = -2
} Direction;

typedef enum {
    EMPTY = 0,
    WALL = 1,
    REGULAR_PILL = 2,
    PORTAL_LEFT = 4,
    PORTAL_RIGHT = 8,
    POWER_PILL = 50
} TileType;

/* constants */
volatile int pacman_lives = 1;
volatile int pacman_status = PAUSE;
volatile int pacman_x;
volatile int pacman_y;
volatile int current_time = INITIAL_TIME;
volatile int current_score;
volatile int direction;
volatile int pacman_score;



/* prototypes */
static void draw_sprite(int x, int y, const uint8_t sprite[TILE_SIZE][TILE_SIZE], uint16_t color);
static void handle_pill_collection(TileType pill_type);
static void clear_tile(int x, int y);
static const uint8_t (*get_pacman_sprite(int direction))[TILE_SIZE];
void display_timer(int current_time);

#endif