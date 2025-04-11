#ifndef  __Maze_h
#define  __Maze_h
#include "../GLCD/GLCD.h"
#include "stdbool.h"
#include "../Utils/Utils.h"

#define MAX_PILLS 6
#define MAX_EXTRA_LIVES 2
#define POINTS_PER_LIFE 1000

/* randomness of power pill values*/
#define POWER_PILL_MIN_TIMER 0
#define POWER_PILL_MAX_TIMER 5
#define POWER_PILL_MULTIPLIER 25000000

/* constants */
volatile int pills_placed;
volatile int countPills;
volatile int power_pill_max_timer = 6;

/* prototypes */
static void draw_tile(int x, int y, uint16_t color);
static void clear_tile(int x, int y);
static bool is_valid_position(int x, int y);


#endif
