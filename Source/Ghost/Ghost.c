#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "Ghost.h"
#include "stdlib.h"

extern uint8_t mazeMatrix[BOARD_HEIGHT][BOARD_WIDTH];

/* pacman variables*/
extern int pacman_y;
extern int pacman_x;
extern int pacman_score;
extern int pacman_lives;
extern int pacman_status;

static const uint8_t GHOST_SPRITE[TILE_SIZE][TILE_SIZE] = {
    {0,0,1,1,1,1,0,0},
    {0,1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1,1},
    {1,1,0,1,1,0,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,1,1,1,1,1,1,1},
    {1,0,1,0,1,0,1,0}
};

void init_position_memory(void) {
    int i;
    for(i = 0; i < MEMORY_SIZE; i++) {
        ghost_memory.positions[i][0] = -1;
        ghost_memory.positions[i][1] = -1;
    }
    ghost_memory.current = 0;
}

void remember_position(int x, int y) {
    ghost_memory.positions[ghost_memory.current][0] = x;
    ghost_memory.positions[ghost_memory.current][1] = y;
    ghost_memory.current = (ghost_memory.current + 1) % MEMORY_SIZE;
}

bool was_recently_visited(int x, int y) {
    int i;
    for(i = 0; i < MEMORY_SIZE; i++) {
        if(ghost_memory.positions[i][0] == x && 
           ghost_memory.positions[i][1] == y) {
            return true;
        }
    }
    return false;
}

void draw_ghost(int x, int y, uint16_t color) {
    uint16_t x_start = x * TILE_SIZE;
    uint16_t y_start = y * TILE_SIZE + OFFSET;
    int i,j;
    for(i = 0; i < TILE_SIZE; i++) {
        for(j = 0; j < TILE_SIZE; j++) {
            if(GHOST_SPRITE[i][j]) {
                LCD_SetPoint(x_start + j, y_start + i, color);
            } else {
                LCD_SetPoint(x_start + j, y_start + i, COLOR_BACKGROUND);
            }
        }
    }
    return;
}

bool check_collision(int pacman_x, int pacman_y) {
    return (blinky_x == pacman_x && blinky_y == pacman_y);
}

int manhattan_distance(int x1, int y1, int x2, int y2) {
    return abs(x2 - x1) + abs(y2 - y1);
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
    if(mazeMatrix[y][x]==2){
        draw_pill(x, y);
    }else if(mazeMatrix[y][x]==50) {
        draw_power_pill(x, y);
    }
}

void get_valid_moves(int x, int y, bool valid_moves[4]) {
    bool is_in_respawn = (y >= RESPAWN_TOP && y <= RESPAWN_BOTTOM && x >= RESPAWN_LEFT && x <= RESPAWN_RIGHT);
    
    valid_moves[UP] = (y > 0 && mazeMatrix[y-1][x] != WALL && (!blinky_hasExitedHouse || !(y-1 >= RESPAWN_TOP && y-1 <= RESPAWN_BOTTOM && x >= RESPAWN_LEFT && x <= RESPAWN_RIGHT)));
    valid_moves[DOWN] = (y < BOARD_HEIGHT-1 && mazeMatrix[y+1][x] != WALL && (!blinky_hasExitedHouse || !(y+1 >= RESPAWN_TOP && y+1 <= RESPAWN_BOTTOM && x >= RESPAWN_LEFT && x <= RESPAWN_RIGHT)));
    valid_moves[LEFT] = (x > 0 && mazeMatrix[y][x-1] != WALL && (!blinky_hasExitedHouse || !(y >= RESPAWN_TOP && y <= RESPAWN_BOTTOM && x-1 >= RESPAWN_LEFT && x-1 <= RESPAWN_RIGHT)));
    valid_moves[RIGHT] = (x < BOARD_WIDTH-1 && mazeMatrix[y][x+1] != WALL && (!blinky_hasExitedHouse || !(y >= RESPAWN_TOP && y <= RESPAWN_BOTTOM && x+1 >= RESPAWN_LEFT && x+1 <= RESPAWN_RIGHT)));
}

int evaluate_move(int x, int y, int target_x, int target_y, bool chase) {
    int distance = manhattan_distance(x, y, target_x, target_y);
    
    if(was_recently_visited(x, y)) {
        return chase ? 9999 : -9999;
    }
    
    if (!chase) {
        int score = distance * 2;  
        int i;
        bool valid_moves[4];
        get_valid_moves(x, y, valid_moves);
        int escape_routes = 0;
        for( i = 0; i < 4; i++) {
            if(valid_moves[i]) escape_routes++;
        }
        
        score += escape_routes * 10;
        
        
        return score; // fleeing
    } else {
        return distance;  // chasing
    }
}

Direction get_random_direction(bool valid_moves[4]) {
    int valid_count = 0;
    int i;
    for(i = 0; i < 4; i++) {
        if(valid_moves[i]) valid_count++;
    }
    
    if(valid_count == 0) return RIGHT;
    int random_choice = rand() % valid_count;
    
    int count = 0;
    for(i = 0; i < 4; i++) {
        if(valid_moves[i]) {
            if(count == random_choice) {
                return (Direction)i;
            }
            count++;
        }
    }
    
	}

Direction get_best_direction(int target_x, int target_y, bool chase) {
    bool valid_moves[4];
    get_valid_moves(blinky_x, blinky_y, valid_moves);
    
    int best_score = chase ? 9999 : -9999;
    Direction best_dir = RIGHT;
    bool found_move = false;
    
    if(valid_moves[UP]) {
        int score = evaluate_move(blinky_x, blinky_y - 1, target_x, target_y, chase);
        if((chase && score < best_score) || (!chase && score > best_score)) {
            best_score = score;
            best_dir = UP;
            found_move = true;
        }
    }
    
    if(valid_moves[DOWN]) {
        int score = evaluate_move(blinky_x, blinky_y + 1, target_x, target_y, chase);
        if((chase && score < best_score) || (!chase && score > best_score)) {
            best_score = score;
            best_dir = DOWN;
            found_move = true;
        }
    }
    
    if(valid_moves[LEFT]) {
        int score = evaluate_move(blinky_x - 1, blinky_y, target_x, target_y, chase);
        if((chase && score < best_score) || (!chase && score > best_score)) {
            best_score = score;
            best_dir = LEFT;
            found_move = true;
        }
    }
    
    if(valid_moves[RIGHT]) {
        int score = evaluate_move(blinky_x + 1, blinky_y, target_x, target_y, chase);
        if((chase && score < best_score) || (!chase && score > best_score)) {
            best_score = score;
            best_dir = RIGHT;
            found_move = true;
        }
    }    
    return best_dir;
}

void step_back_from_pacman(void) {
    int dx = blinky_x - pacman_x;
    int dy = blinky_y - pacman_y;
    
    bool valid_moves[4];
    get_valid_moves(blinky_x, blinky_y, valid_moves);
    
    if (abs(dx) > abs(dy)) {
        if (dx > 0 && valid_moves[RIGHT]) {
            blinky_x++;
        } else if (dx < 0 && valid_moves[LEFT]) {
            blinky_x--;
        } else if (dy > 0 && valid_moves[DOWN]) {
            blinky_y++;
        } else if (dy < 0 && valid_moves[UP]) {
            blinky_y--;
        }
    } else {
        if (dy > 0 && valid_moves[DOWN]) {
            blinky_y++;
        } else if (dy < 0 && valid_moves[UP]) {
            blinky_y--;
        } else if (dx > 0 && valid_moves[RIGHT]) {
            blinky_x++;
        } else if (dx < 0 && valid_moves[LEFT]) {
            blinky_x--;
        }
    }
}

void update_ghost_direction(void) {
    if (!blinky_isActive) return;
    
    Direction dir;
    bool valid_moves[4];
    get_valid_moves(blinky_x, blinky_y, valid_moves);
    
    bool is_in_house = (blinky_y >= RESPAWN_TOP && blinky_y <= RESPAWN_BOTTOM && 
                       blinky_x >= RESPAWN_LEFT && blinky_x <= RESPAWN_RIGHT);
    
    if (!blinky_hasExitedHouse) {
        int center_x = (RESPAWN_LEFT + RESPAWN_RIGHT) / 2;
        
        if (blinky_x < center_x && valid_moves[RIGHT]) {
            dir = RIGHT;
        } else if (blinky_x > center_x && valid_moves[LEFT]) {
            dir = LEFT;
        }
        else if (valid_moves[UP]) {
            dir = UP;
        }
        else if (valid_moves[DOWN]) {
            dir = DOWN;
        }

    } else {
        if (blinky_isOnCooldown) {
            dir = get_random_direction(valid_moves);
        } else if (blinky_state == FRIGHTENED) {
            dir = get_best_direction(pacman_x, pacman_y, false);
        } else if (blinky_state == CHASE) {
            dir = get_best_direction(pacman_x, pacman_y, true);
        }
    }
    
    switch(dir) {
        case UP:    blinky_y--; break;
        case DOWN:  blinky_y++; break;
        case LEFT:  blinky_x--; break;
        case RIGHT: blinky_x++; break;
    }
}
void move_ghost(void) {
    if(!blinky_isActive) return;
    
    int old_x = blinky_x, old_y = blinky_y;
     if(!blinky_hasExitedHouse && !(blinky_y >= RESPAWN_TOP && blinky_y <= RESPAWN_BOTTOM && 
        blinky_x >= RESPAWN_LEFT && blinky_x <= RESPAWN_RIGHT)) {
        blinky_hasExitedHouse = true;
    }   
				
		update_ghost_direction();
    remember_position(blinky_x, blinky_y);
    if(check_collision(pacman_x, pacman_y)) {
        if(blinky_state == FRIGHTENED) {
            pacman_score += GHOST_POINTS;
            clear_tile(old_x, old_y);
            blinky_isActive = false;
            reset_timer(3);
            init_timer(3, 0, 3, 1, RESPAWN_TIME);
            enable_timer(3);
            return;
        } else {
            if(pacman_lives > 1) {
                pacman_lives--;
                display_lives(true);
                step_back_from_pacman();
                blinky_isOnCooldown = true;
                init_timer(3, 0, 2, 1, GHOST_COOLDOWN_TIME);
            } else {
                pacman_status = GAMEOVER;
                change_status();
            }
        }
    }
    
    uint16_t color = (blinky_state == CHASE) ? GHOST_NORMAL_COLOR : GHOST_FRIGHTENED_COLOR;
    draw_ghost(blinky_x, blinky_y, color);
    clear_tile(old_x, old_y);
}

void handle_power_pill_ghost(void) {
    blinky_state = FRIGHTENED;
    blinky_speed = GHOST_SPEED_NORMAL;
    init_timer(3,0,1,3, FRIGHTENED_TIME);    
}

/* callbacks */ 
void ghost_respawn_callback(void) {
    blinky_isActive = true;
    blinky_x = SPAWN_X;
    blinky_y = SPAWN_Y;
    blinky_state = CHASE;
    init_position_memory();  
		blinky_hasExitedHouse=false;
    draw_ghost(blinky_x,blinky_y,Red);
}

void frightened_end_callback(void) {
    if (blinky_state == FRIGHTENED) {
        blinky_state = CHASE;
        blinky_speed = GHOST_SPEED_NORMAL;
				init_position_memory(); 
    }
}

void ghost_cooldown_end_callback(void) {
    blinky_isOnCooldown = false;
    init_position_memory();  
}