/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "TouchPanel/TouchPanel.h"
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "button/button.h"
#include "../adc/adc.h"
#include "../music/music.h"

#ifdef SIMULATOR
extern uint8_t ScaleFlag; // <- ScaleFlag needs to visible in order for the emulator to find the symbol (can be placed also inside system_LPC17xx.h but since it is RO, it needs more work)
#endif	
int main(void){
	/* System Initialization (i.e., PLL)  */
	SystemInit();
	BUTTON_init();
	ADC_init();
	CAN_Init();

	
	/*SPEAKER PIN SETTING*/
	LPC_PINCON->PINSEL1 |= (1<<21);
	LPC_PINCON->PINSEL1 &= ~(1<<20);
	LPC_GPIO0->FIODIR |= (1<<26);
	
	/* TIMERS POWER UP*/
  LPC_SC -> PCONP |= (1<<22); // TIM2
  LPC_SC -> PCONP |= (1<<23); // TIM3
	
	/* Display INIT*/
	LCD_Initialization();
	LCD_Clear(Black);
	
	/* Joystick INIT*/
	joystick_init();
	
	/* Display SETUP */
	init_header();
	setup_gamespace();
	init_footer();

	
	/*PACMAN INIT*/
	init_pacman();
	
	/*RIT INIT*/
	init_RIT(0x4c4b40); 
	enable_RIT();
		
	/* Define pause status at the beginning*/
	change_status();
	
	
	/* power-down	mode */
	LPC_SC->PCON |= 0x1;								
	LPC_SC->PCON &= ~(0x2);						
	
  while (1)	
  {
		__ASM("wfi");
  }
}

/*********************************************************************************************************
      END FILE
*********************************************************************************************************/