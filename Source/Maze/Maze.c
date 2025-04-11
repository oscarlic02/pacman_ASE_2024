#include "stdio.h"
#include "string.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../Utils/Utils.h"
#include "../adc/adc.h"
#include "Maze.h"
uint8_t mazeMatrix[BOARD_HEIGHT][BOARD_WIDTH];

/* pacman variables*/
extern int pacman_status;
extern int pacman_x;
extern int pacman_y;
extern int  pacman_lives;
extern int pacman_score;

/*ghost variables (blinky)*/
extern int blinky_x;
extern int blinky_y;

static const uint8_t INITIAL_MAZE[BOARD_HEIGHT][BOARD_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,3,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,2,1,1,2,1,1,1,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,1,1,1,2,1,1,2,1,1,1,1,1,0,1,1,1,1,1,1},
		{1,1,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1},
												/* RECTANGLE */ 
    {1,1,1,1,1,1,0,1,1,0, 1,1,1,0,0,1,1,1, 0,1,1,0,1,1,1,1,1,1},
    {1,1,1,1,1,1,0,1,1,0, 1,0,0,5,0,0,0,1, 0,1,1,0,1,1,1,1,1,1},
		{8,0,0,0,0,0,0,0,0,0, 1,0,0,0,0,0,0,1, 0,0,0,0,0,0,0,0,0,4},
    {1,1,1,1,1,1,2,1,1,0, 1,0,0,0,0,0,0,1, 0,1,1,0,1,1,1,1,1,1},
		
    {1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,0,0,0,0,0,0,0,0,0,0,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
    {1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,1,1,1,1,2,1,1,1,1,1,2,1,1,2,1,1,1,1,1,2,1,1,1,1,2,1},
    {1,2,2,2,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,2,2,2,1},
    {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
    {1,1,1,2,1,1,2,1,1,2,1,1,1,1,1,1,1,1,2,1,1,2,1,1,2,1,1,1},
    {1,2,2,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,1,1,2,2,2,2,2,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,1,1,1,1,1,1,1,1,1,1,2,1,1,2,1,1,1,1,1,1,1,1,1,1,2,1},
    {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

static bool is_valid_position(int x, int y) {
    return x >= 0 && x < BOARD_WIDTH && 
           y >= 0 && y < BOARD_HEIGHT && 
           mazeMatrix[y][x] == REGULAR_PILL;
}

void init_header(void) {
    const char* texts[] = {
        "Game Over in", "60s", "Score", "0"
    };
    const int positions[][2] = {{0,0}, {0,20}, {190,0}, {190,20}};
		int i;
    for(i = 0; i < 4; i++) {
        GUI_Text(positions[i][0], positions[i][1], 
                (unsigned char*)texts[i], COLOR_TEXT, COLOR_BACKGROUND);
    }
}

static void draw_tile(int x, int y, uint16_t color) {
    uint16_t x_start = x * TILE_SIZE;
    uint16_t y_start = y * TILE_SIZE + OFFSET;
    int i,j;
    for(i = 0; i < TILE_SIZE; i++) {
        for(j = 0; j < TILE_SIZE; j++) {
            LCD_SetPoint(x_start + j, y_start + i, color);
        }
    }
}

static void init_timers_power_pills(){
init_timer(3,0,0,1,0x003D090); // 10 ms generation of power pills
}

void setup_gamespace() {
int x,y,i,j;
memcpy(mazeMatrix, INITIAL_MAZE, sizeof(mazeMatrix));
    for(y = 0; y < BOARD_HEIGHT; y++) {
        for(x = 0; x < BOARD_WIDTH; x++) {
            switch(mazeMatrix[y][x]) {
                case WALL:
                    draw_tile(x, y, COLOR_WALL);
                    break;
                    
                case REGULAR_PILL:
                    draw_pill(x, y);
                    break;
								
                case PACMAN_START:
                    pacman_x = x;
                    pacman_y = y;
                    draw_pacman(1, 1, 0);
                    break;
								case BLINKY_GHOST:
                    blinky_x = x;
                    blinky_y = y;
                    draw_ghost(blinky_x, blinky_y, Red);
                    break;
            }
        }
    }
	init_timers_power_pills();
	pills_placed = generate_power_pills();
}



void redraw_rectangle() {
    const int RECT_BOUNDS[] = {12, 15, 10, 17}; // top, bottom, left, right
    int x,y;
    for( y = RECT_BOUNDS[0]; y <= RECT_BOUNDS[1]; y++) {
        for(x = RECT_BOUNDS[2]; x <= RECT_BOUNDS[3]; x++) {
            uint16_t color = (mazeMatrix[y][x] == WALL) ? COLOR_WALL : COLOR_BACKGROUND;
            draw_tile(x, y, color);
        }
    }
}

void write_status() {
    const char* messages[] = {"", "PAUSE", "GAME OVER", "VICTORY"};
    const int x_positions[] = {0, 91, 83, 90};
    
    if(pacman_status == RESUME) {
        redraw_rectangle();
				draw_ghost(blinky_x,blinky_y,Red);
        return;
    }
    
    GUI_Text(x_positions[pacman_status], 160, 
            (unsigned char*)messages[pacman_status], 
            COLOR_TEXT, COLOR_BACKGROUND);
}

void display_lives(bool ghost_confict) {
int y,x,i;
if(ghost_confict==true){
	for (i = 0; i < pacman_lives; i++) {
				 draw_pacman(10 + (i * 20),310,1);
    }
}else{
int lives = (pacman_score / 1000) + 1; 
if(lives!=pacman_lives){
	if (lives > 3) {
        lives = 3;
    }
		if(lives>0){
		pacman_lives=lives;
		}
    for (i = 0; i < pacman_lives; i++) {
				 draw_pacman(10 + (i * 20),310,1);
    }
}
}
		check_win();
}

void init_footer(void) {
    GUI_Text(0, 300, (unsigned char*)"Lives:", COLOR_TEXT, COLOR_BACKGROUND);
		display_lives(false);
}


int generate_power_pills() {
int x,y,i,j;
		if(pills_placed == MAX_PILLS) {
        disable_timer(3);
        return MAX_PILLS;
    }
		// randomness in terms of time
    int seed = rand() % (power_pill_max_timer - POWER_PILL_MIN_TIMER + 1);
    int timer_value = seed * POWER_PILL_MULTIPLIER + pacman_score;
		reset_timer(3);
    init_timer(3, 0, 0, 3, timer_value);
		enable_timer(3);
		
		int random =  rand()%(27-0 + 1);
		srand((LPC_TIM3->TCR ^ LPC_TIM1->TC ^ LPC_TIM0->TC ^ LPC_TIM2->TC ^ random));
        do{
				// random positions
				 x =  rand()%(27-0 + 1);
				 y =  rand()%(30-0+1);
				}
				while(mazeMatrix[y][x] != 2);
				mazeMatrix[y][x] = POWER_PILL;
				draw_power_pill(x, y);
			return ++pills_placed;	

}
