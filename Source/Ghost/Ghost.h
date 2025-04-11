#ifndef  __Ghost_h
#define  __Ghost_h

#include "GLCD/GLCD.h"
#include "Utils/Utils.h"
#include "stdbool.h"
#include "stdio.h"

#define GHOST_SPEED_NORMAL 0x00BEBC20 // same speed as pacman  
#define GHOST_SPEED_FAST 0x00098968 // half of above speed
#define FRIGHTENED_TIME 0x0EE6B280   // 10 milliseconds at 25 Mhz timer (K=0.01*25.000.000 = 250.000 => 0x0003D090)
#define RESPAWN_TIME 0x000124F8 // 3 milliseconds at 25Mhz timer (K=0.03*25.000.000=75.000 => 0x00124F8)
#define GHOST_POINTS 100        
#define MAX_PATH_LENGTH 100    
#define SPAWN_X 13           
#define SPAWN_Y 13
#define GHOST_NORMAL_COLOR Red    
#define GHOST_FRIGHTENED_COLOR Blue2 
#define GHOST_COOLDOWN_TIME 0x000124F8
#define MEMORY_SIZE 4
/* House rectange coordinates of the ghost */
#define RESPAWN_TOP 12   
#define RESPAWN_BOTTOM 15 
#define RESPAWN_LEFT 10  
#define RESPAWN_RIGHT 17 
#define CHASE 0
#define FRIGHTENED 1
#define RESPAWNING 2
#define RESPAWN_TIME 0x000124F8 // 3 milliseconds at 25Mhz timer (K=0.03*25.000.000=75.000 => 0x00124F8)




typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT
} Direction;

typedef enum {
    COLOR_GHOST_NORMAL = Red,
    COLOR_GHOST_FRIGHTENED = Blue,
} GhostColors;


typedef struct Node {
    int x, y;
    int g_cost;  
    int h_cost;  
    int f_cost;  
    struct Node* parent;
} Node;
  

typedef struct {
    int positions[MEMORY_SIZE][2];  
    int current;                  
} PositionMemory;

static PositionMemory ghost_memory = {{0}};

/* constants */
volatile int blinky_x = SPAWN_X;
volatile int blinky_y = SPAWN_Y;
volatile int blinky_state = CHASE;
volatile int blinky_speed = GHOST_SPEED_NORMAL;
volatile bool blinky_isActive = true;
volatile Node* blinky_path = NULL;
volatile int blinky_path_length = 0;
volatile int blinky_current_path_index=0;
volatile bool blinky_hasExitedHouse = false;
volatile bool blinky_isOnCooldown = false;
/* prototypes */
void init_ghost();
void draw_ghost(int x, int y, uint16_t color);
void move_ghost();
void handle_power_pill_ghost();
void ghost_cooldown_end_callback();
void ghost_respawn_callback();
#endif