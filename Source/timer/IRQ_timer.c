/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "timer.h"
#include "../GLCD/GLCD.h"
#include "../CAN/CAN.h"

// #include "../music/music.h"

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
extern unsigned char led_value;					/* defined in funct_led								*/

unsigned char ledval = 0xA5;

extern int status;
extern blinky_isActive;
volatile int counting = 0;
volatile int speed_ghost = 0;
extern int blinky_state;
volatile int count_blinky_frightened = 60;

#define UPTICKS 1



volatile int test;

uint16_t SinTable[45] =                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};
void TIMER0_IRQHandler (void)
{
	if(LPC_TIM0->IR & 1) // MR0
	{
		static int sineticks=0;
		/* DAC management */	
		static int currentValue; 
		currentValue = SinTable[sineticks];
		currentValue -= 410;
		currentValue /= 1;
		currentValue += 410;
		LPC_DAC->DACR = currentValue <<6;
		sineticks++;
		if(sineticks==45) sineticks=0;

		LPC_TIM0->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM0->IR & 2){ // MR1
		LPC_TIM0->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 4){ // MR2
		LPC_TIM0->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM0->IR & 8){ // MR3
		LPC_TIM0->IR = 8;			// clear interrupt flag 
	}
  return;
}

/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
void TIMER1_IRQHandler (void)
{
	
  if(LPC_TIM1->IR & 1) // MR0
    {
        move_pacman();
        counting++;
        pacman_moves++;
        total_game_time++;
        
        if(counting == 10){
            update_timer();
            counting = 0;
        }
				if(blinky_state==1){
				   count_blinky_frightened--;
				}else{
					count_blinky_frightened=60;
				}
				if(count_blinky_frightened==0){
						frightened_end_callback();
				}
				
        
				if(blinky_isActive){
            // One ghost move every three pacman moves
            if(total_game_time < PHASE_1_TIME){
                if(pacman_moves >= 3){
                    move_ghost();
                    pacman_moves = 0;
                }
            }
            // One ghost move every two pacman moves
            else if(total_game_time < PHASE_2_TIME){
                if(current_phase == 1){
                    current_phase = 2;
                }
                if(pacman_moves >= 2){
                    move_ghost();
                    pacman_moves = 0;
                }
            }
           // Ghost moves with pacman (1:1)
            else if(total_game_time < PHASE_3_TIME){
                if(current_phase == 2){
                    current_phase = 3;
                }
                move_ghost();
                pacman_moves = 0;
            }
            // Ghost moves twice for each pacman move
            else {
                if(current_phase == 3){
                    current_phase = 4;
                }
                move_ghost();
                pacman_moves = 0;
            }
        }
		handle_CAN_values();
		LPC_TIM1->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM1->IR & 2){ // MR1
		LPC_TIM1->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 4){ // MR2
		// your code	
		LPC_TIM1->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 8){ // MR3
		// your code	
		LPC_TIM1->IR = 8;			// clear interrupt flag 
	} 

	return;
}

/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER2_IRQHandler (void)
{
	if(LPC_TIM2->IR & 1) // MR0
	{
		disable_timer(0);
		LPC_TIM2->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM2->IR & 2){ // MR1
		
		LPC_TIM2->IR = 2;			// clear		interrupt flag 
	}
	else if(LPC_TIM1->IR & 4){ // MR2
		
		// your code	
		LPC_TIM2->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM1->IR & 8){ // MR3
		// your code	
		LPC_TIM2->IR = 8;			// clear interrupt flag 
	} 

  return;
}


/******************************************************************************
** Function name:		Timer2_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void TIMER3_IRQHandler (void)
{
	if(LPC_TIM3->IR & 1) // MR0
	{
		generate_power_pills();
		LPC_TIM3->IR = 1;			//clear interrupt flag
	}
	else if(LPC_TIM3->IR & 2){ // MR1
		LPC_TIM3->IR = 2;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 4){ // MR2
		ghost_cooldown_end_callback();
		LPC_TIM3->IR = 4;			// clear interrupt flag 
	}
	else if(LPC_TIM3->IR & 8){ // MR3
		if(!blinky_isActive){
			ghost_respawn_callback();
		}
		LPC_TIM3->IR = 8;			// clear interrupt flag 
	} 

  return;
}
/******************************************************************************
**                            End Of File
******************************************************************************/