#include "music.h"
#include "../timer/timer.h"
#include "LPC17xx.h" 

volatile int current_length;


void playNote(NOTE note)
{
    if(note.freq != pause)
    {
        reset_timer(0);
        init_timer(0, 0, 0, 3, note.freq);
        enable_timer(0);
    }
    reset_timer(2);
    init_timer(2, 0, 0, 7, note.duration);
    enable_timer(2);
}

BOOL isNotePlaying(void) {
    return ((LPC_TIM0->TCR !=0) || (LPC_TIM2->TCR!=0));
}

void stop_sound(void) {
    sound_playing = FALSE;
    note_playing = FALSE;
    disable_timer(0);
    disable_timer(2);
    current_sound = SOUND_NONE;
}

void play_next_note(void) {
    if (!sound_playing) return;
    
    switch(current_sound) {
        case SOUND_BACKGROUND:
            current_melody = background_melody;
            current_length = background_length;
            break;
        case SOUND_PILL:
            current_melody = pill_melody;
            current_length = pill_length;
            break;
        case SOUND_POWER_PILL:
            current_melody = power_pill_melody;
            current_length = power_pill_length;
            break;
        case SOUND_GAMEOVER:
            current_melody = gameover_melody;
            current_length = gameover_length;
            break;
        case SOUND_VICTORY:
            current_melody = victory_melody;
            current_length = victory_length;
            break;
        default:
            return;
    }
    
    if (note_index < current_length) {
        playNote(current_melody[note_index]);
        note_index++;
    } else {
        stop_sound();
    }
}


void play_pill_sound(void) {
        current_sound = SOUND_PILL;
        note_index = 0;
        sound_playing = TRUE;
        play_next_note();
    
}

void play_power_pill_sound(void) {
        current_sound = SOUND_POWER_PILL;
        note_index = 0;
        sound_playing = TRUE;
        play_next_note();
    
}