#include "Pacman.h"
#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../adc/adc.h"

extern uint8_t mazeMatrix[BOARD_HEIGHT][BOARD_WIDTH];

/* variables of blinky */

extern blinky_state;
extern blinky_isActive;
extern blinky_x;
extern blinky_y;
extern blinky_isOnCooldown;

static const uint8_t PACMAN_SPRITES[4][TILE_SIZE][TILE_SIZE] = {
    // RIGHT
    {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 0, 0, 0, 0},
        {1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0}
    },
    // LEFT
    {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 1, 1, 1},
        {0, 0, 0, 0, 1, 1, 1, 1},
        {0, 0, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0}
    },
    // UP
    {
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 0, 0}
    },
    // DOWN
    {
        {0, 0, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 0, 0, 0, 1, 1, 1},
        {0, 1, 0, 0, 0, 0, 1, 0},
        {0, 0, 0, 0, 0, 0, 0, 0}
    }
};

void init_pacman() {
    pacman_x = INITIAL_POSITION;
    pacman_y = INITIAL_POSITION;
    pacman_score = 0;
		init_timer(1,0,0,3,0x6D59F8);  // speed pacman
}

void change_status(void) {
int i;
    switch(pacman_status) {
        case PAUSE:
            write_status(PAUSE);
            for(i = 0; i < 4; i++) {
                disable_timer(i);
            }
            pacman_status = RESUME;
            break;
            
        case RESUME:
            write_status(RESUME);
            for(i = 0; i < 4; i++) {
                enable_timer(i);
            }
            pacman_status = PAUSE;
            break;
            
        case GAMEOVER:
						write_status(GAMEOVER);
            for(i = 0; i < 4; i++) {
                disable_timer(i);
            }
						break;
        case VICTORY:
            write_status(VICTORY);
            for(i = 0; i < 4; i++) {
                disable_timer(i);
            }
            break;
    }
    reset_RIT();
}

void update_timer() {
    if (current_time <= 0) {
        pacman_status = GAMEOVER;
        change_status();
        return;
    }    
    current_time--;
}

void display_timer(int current_time){
char timer_value[5];
sprintf(timer_value, current_time < 10 ? "0%d" : "%d", current_time);
    GUI_Text(0, 20, (unsigned char*)timer_value, 0xFFFF, Black);
}

void update_score() {
  /*  char score_value[10];
    sprintf(score_value, "%d", pacman_score);
    GUI_Text(190, 20, (unsigned char*)score_value, 0xFFFF, Black);
*/}

bool is_valid_move(int new_x, int new_y) {
    return mazeMatrix[new_y][new_x] != WALL;
}

void check_win() {
    if (pacman_score >= MAX_SCORE) {
        pacman_status = VICTORY;
        change_status();
    }
}

static const uint8_t (*get_pacman_sprite(int direction))[TILE_SIZE] {
    switch (direction) {
        case UP: return PACMAN_SPRITES[2];
        case DOWN: return PACMAN_SPRITES[3];
        case RIGHT: return PACMAN_SPRITES[0];
        case LEFT: return PACMAN_SPRITES[1];
        default: return PACMAN_SPRITES[0];
    }
}

static void draw_sprite(int x, int y, const uint8_t sprite[TILE_SIZE][TILE_SIZE], uint16_t color) {
int i,j;    
	for (i = 0; i < TILE_SIZE; i++) {
        for (j = 0; j < TILE_SIZE; j++) {
            LCD_SetPoint(x + j, y + i, sprite[i][j] ? color : Black);
        }
    }
}

void draw_pacman(int x, int y, int life) {
    if (life) {
        // Draw remaining lives
        int base_y = 305;
        int spacing = 12;
        int z;
        for (z = 0; z < pacman_lives; z++) {
            draw_sprite(52 + (z * spacing), base_y, PACMAN_SPRITES[0], Yellow);
        }
    } else {
        // Draw active Pacman
        uint16_t x_start = pacman_x * TILE_SIZE;
        uint16_t y_start = (pacman_y * TILE_SIZE) + OFFSET;
        draw_sprite(x_start, y_start, get_pacman_sprite(direction), Yellow);
    }
}

static void handle_pill_collection(TileType pill_type) {
    switch (pill_type) {
        case REGULAR_PILL:
            pacman_score += REGULAR_PILL_SCORE;
				    play_pill_sound();
            break;
        case POWER_PILL:
            pacman_score += POWER_PILL_SCORE;
						handle_power_pill_ghost();
				    play_power_pill_sound();
            break;
        default:
            return;
    }
    update_score();
    display_lives(false);
}

static void clear_tile(int x, int y) {
    int x_pos = x * TILE_SIZE;
    int y_pos = y * TILE_SIZE + OFFSET;
    int i,j;
    for (i = 0; i < TILE_SIZE; i++) {
        for (j = 0; j < TILE_SIZE; j++) {
            LCD_SetPoint(x_pos + j, y_pos + i, Black);
        }
    }
}


void move_pacman() {
    if (direction == 0) return;
    
    int new_x = pacman_x;
    int new_y = pacman_y;
    
    switch(direction) {
        case UP: new_y--; break;
        case DOWN: new_y++; break;
        case RIGHT: new_x++; break;
        case LEFT: new_x--; break;
    }
    
    if (new_x >= BOARD_WIDTH) {
        new_x = 0;
    } else if (new_x < 0) {
        new_x = BOARD_WIDTH - 1;
    }
    
    if (!is_valid_move(new_x, new_y)) return;
    
    if (blinky_isActive && new_x == blinky_x && new_y == blinky_y) {
        if (blinky_state == FRIGHTENED) {
            pacman_score += 100;
            clear_tile(blinky_x, blinky_y);
            blinky_isActive = false;
            reset_timer(3);
            init_timer(3, 0, 3, 1, RESPAWN_TIME);
            enable_timer(3);
        } else {
            if (pacman_lives > 1) {
                pacman_lives--;
                display_lives(true);
                new_x = pacman_x;  
                new_y = pacman_y;
            } else {
                pacman_status = GAMEOVER;
                change_status();
                return;
            }
        }
    }
    
    TileType tile_type = mazeMatrix[new_y][new_x];
    handle_pill_collection(tile_type);
    
    // portal
    if (tile_type == PORTAL_LEFT) {
        new_x = 0;
    } else if (tile_type == PORTAL_RIGHT) {
        new_x = BOARD_WIDTH - 1;
    }
    
    mazeMatrix[new_y][new_x] = EMPTY;
    clear_tile(pacman_x, pacman_y);
    pacman_x = new_x;
    pacman_y = new_y;
    draw_pacman(0, 0, 0);
}