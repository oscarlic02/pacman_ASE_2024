/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../GLCD/GLCD.h"
#include "../timer/timer.h"
#include "../music/music.h"
#include "../CAN/CAN.h"

#define UPTICKS 1
#define RIT_SEMININIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
volatile int down_0 = 0;
volatile int down_1 = 0;
volatile int down_2 = 0;
volatile int select = 0;
volatile int up = 0;
volatile int down = 0;
volatile int left = 0;
volatile int right = 0;
volatile int start=1;
volatile int delay_song=0;

extern int pacman_status;
extern int pacman_score;
extern int direction;

void RIT_IRQHandler (void)
{	
/* MUSIC MANAGEMENT */
if(pacman_status==0 && delay_song%3 == 0) {  // delay to improve song quality (enlarge note sounds)
	
++ticks;
if(ticks == UPTICKS){
   ticks=0;

	if(wait==0) {
	 playNote(background_melody[currentNote++]);
	}
}

if(currentNote== (sizeof(background_melody) / sizeof(background_melody[0]))){
		currentNote=0;
}

}


if(pacman_status==2 && delay_song%4 == 0) {  // delay to improve song quality (enlarge note sounds)
	
++ticks;
if(ticks == UPTICKS){
   ticks=0;
	 playNote(gameover_melody[currentNote++]);
}

if(currentNote== (sizeof(background_melody) / sizeof(background_melody[0]))){
		currentNote=0;
}

}

if(pacman_status==3 && delay_song%4 == 0) {  // delay to improve song quality (enlarge note sounds)
	
++ticks;
if(ticks == UPTICKS){
   ticks=0;
	 playNote(victory_melody[currentNote++]);
}

if(currentNote== (sizeof(background_melody) / sizeof(background_melody[0]))){
		currentNote=0;
}


}
delay_song++;

	
/* Static as its value persists between calls to the function. It is not reinitialized each time the function is executed.*/
static uint8_t position=0;
/*************************INT0***************************/
if(down_0 !=0){
	down_0++;
	if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){
		switch(down_0){
			case 2:
				change_status();
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_0=0;			
		NVIC_EnableIRQ(EINT0_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
	}
} // end INT0

/*************************KEY1***************************/
if(down_1 !=0){
	down_1++;
	if((LPC_GPIO2->FIOPIN & (1<<11)) == 0){  /* IDEA: Check the value of the pin at position 11. Why? Because in the schematic of the BUTTON KEY1 is the schematic 2.11
																							if the value is 0, then the pin was down thus the button was pressed*/
		switch(down_1){
			case 2:				
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_1=0;			
		NVIC_EnableIRQ(EINT1_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
	}
} // end KEY1

/*************************KEY2***************************/
if(down_2 !=0){
	down_2++;
	if((LPC_GPIO2->FIOPIN & (1<<12)) == 0){
		switch(down_2){
			case 2:
				break;
			default:
				break;
		}
	}
	else {	/* button released */
		down_2=0;		
		NVIC_EnableIRQ(EINT2_IRQn);							 /* disable Button interrupts			*/
		LPC_PINCON->PINSEL4    |= (1 << 24);     /* External interrupt 0 pin selection */
	}
} // end KEY2

	/*---------- JOYSTICK -----------  */

	// joystick is selected
	if((LPC_GPIO1->FIOPIN & (1<<25)) == 0){
		select++;	
		switch(select){
				case 1:
				// your code here
				break;
				default:
				break;
			}
	}else{
	select = 0;
	}
	
	// joystick down
	if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
			down++;	
				switch(down){
				case 1:
				direction=-1;
				break;
				default:
				break;
			}
	}
	else{
	down = 0;
	}
	// joystick up
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){
		up++;	
			switch(up){
				case 1:
					direction=1;
				break;
				default:
				break;
			}
	}else{
	up = 0;
	}
	// joystick left
	if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		left++;	
			switch(left){
				case 1:
					direction=-2;
					break;
				default:
				break;
			}
	}else{
	left = 0;
	}
	
	// joystick right
	if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		right++;	
				switch(right){
				case 1:
					direction=2;
					break;
				default:
				break;
			}
	}else{
	right = 0;
	}
	reset_RIT();
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
