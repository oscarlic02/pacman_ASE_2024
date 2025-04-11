#ifndef MUSIC_H
#define MUSIC_H
// partendo da sinistra, il primo vuoto, collego il primo filo con il secondo posto e l'altra estramita al quarto posto 
// 2-4 un filo e 3-5 l'altro filo
//Default: 1.65
#define SPEEDUP 4 // this enlarge the duration of the note

#define TIMERSCALER 1

#define SECOND 0x17D7840 * TIMERSCALER

#include <stdbool.h>
typedef char BOOL;
#define TRUE 1
#define FALSE 0
#define TIMER_FREQUENCY 25000000  // 25 MHz (timer frequency)
#define NOTE_DIVISOR 45  	
#define N_ENTRY(note) (TIMER_FREQUENCY / ((note) * NOTE_DIVISOR)) 
typedef enum note_durations
{
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), // 1/128
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), // 1/64
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), // 1/32
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), // 1/16
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), // 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), // 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), // 1
} NOTE_DURATION;


typedef enum frequencies
{
	A2B = 5351,	// 103Hz	k=5351 a2b
	B2 = 4500,	// 123Hz	k=4500 b2
	C3B = 4370,	// 127Hz	k)4370 c3b
	C3 = 4240,	// 131Hz	k=4240 c3
	D3 = 3779,	// 147Hz	k=3779 d3
	E3 = 3367,	// 165Hz	k=3367 e3
	F3 = 3175,	// 175Hz	k=3175 f3
	G3 = 2834,	// 196Hz	k=2834 g3
	A3B = 2670, // 208Hz  k=2670 a4b
	A3 = 2525,	// 220Hz	k=2525 a3
	B3 = 2249,	// 247Hz	k=2249 b3
	C4 = 2120,	// 262Hz	k=2120 c4
	D4 = 1890,	// 294Hz	k=1890 d4
	E4 = 1684,	// 330Hz	k=1684 e4
	F4 = 1592,	// 349Hz	k=1592 f4
	G4 = 1417,	// 392Hz	k=1417 g4
	A4 = 1263,	// 440Hz	k=1263 a4
	B4 = 1125,	// 494Hz	k=1125 b4
	C5 = 1062, 	// 523Hz	k=1062 c5
	pause = 0		// DO NOT SOUND
} FREQUENCY;

typedef struct 
{
	FREQUENCY freq;
	NOTE_DURATION duration;
} NOTE;

void playNote(NOTE note);
BOOL isNotePlaying(void);


typedef enum {
    SOUND_NONE,
    SOUND_BACKGROUND,
    SOUND_PILL,
    SOUND_POWER_PILL,
    SOUND_PAUSE,
    SOUND_GAMEOVER,
    SOUND_VICTORY
} SoundState;

/* constants */
static SoundState current_sound = SOUND_NONE;
static int note_index = 0;
static bool sound_playing = false;
static BOOL note_playing = FALSE;

static const NOTE background_melody[] = {
    {B4, time_semicroma},
    {F4, time_semicroma},
    {D4, time_semicroma},
    {B4, time_croma},
    {F4, time_semicroma},
    {D4, time_semiminima},
    {C4, time_semicroma},
    {C5, time_semicroma},
    {G4, time_semicroma},
    {E4, time_semicroma},
    {C5, time_croma},
    {G4, time_semicroma},
    {E4, time_semiminima},
		{pause, time_semicroma},
    
};
static const int background_length = 16;

static const NOTE gameover_melody[] = {
    {B3, time_semicroma},
    {A3, time_semicroma},
    {G3, time_semicroma},
    {F3, time_semicroma},
    {pause, time_semicroma},
    {E3, time_croma},
    {D3, time_croma},
    {C3, time_semiminima},
    {B2, time_minima}
};

static const int gameover_length = 9;

static const NOTE pill_melody[] = {
    {B4, time_semibiscroma},  // D3 time_semicroma
    {G4, time_semibiscroma}  // 
};
static const int pill_length = 2;

static const NOTE power_pill_melody[] = {
    {B4, time_semicroma},
    {pause, time_semibiscroma},
    {B4, time_semicroma},
    {pause, time_semibiscroma},
    {B4, time_semicroma},
    {pause, time_semibiscroma},
    {B4, time_semicroma}
};
static const int power_pill_length = 7;

static const NOTE pause_melody[] = {
    {E4, time_croma},
    {A4, time_semiminima},
    {E4, time_croma}
};
static const int pause_length = 3;

static const NOTE victory_melody[] = {
    {C4, time_croma},
    {E4, time_croma},
    {G4, time_croma},
    {C4, time_croma},
    {E4, time_croma},
    {G4, time_croma},
    {B4, time_minima}
};
static const int victory_length = 7;
static volatile int currentNote = 0;
static volatile int ticks = 0; 
static volatile NOTE* current_melody;
static int wait=0;

/*prototypes*/
void play_background_music(void);
BOOL isNotePlaying(void);
#endif
/* EOF */
