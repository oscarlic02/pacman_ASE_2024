#include "stdbool.h"
#include "stdio.h"
#include "string.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../RIT/RIT.h"
#include "../Utils/Utils.h"

void draw_pill(int x, int y) {
    uint16_t x_start = x * TILE_SIZE + 2;
    uint16_t y_start = y * TILE_SIZE + OFFSET + 2;
    int i,j;
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
            LCD_SetPoint(x_start + j, y_start + i, COLOR_REGULAR_PILL);
        }
    }
}

void draw_power_pill(int x, int y) {
    uint16_t x_start = x * TILE_SIZE + 1;
    uint16_t y_start = y * TILE_SIZE + OFFSET + 1;
    int i,j;
    for(i = 0; i < 6; i++) {
        for(j = 0; j < 6; j++) {
            LCD_SetPoint(x_start + j, y_start + i, COLOR_POWER_PILL);
        }
    }
}
