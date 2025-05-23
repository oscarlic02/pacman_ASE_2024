/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/

void joystick_init(void) {
	
	// JOYSTICK UP 
  LPC_PINCON->PINSEL3 &= ~(3<<26);	//PIN mode GPIO (00b value per P1.29)
	LPC_GPIO1->FIODIR   &= ~(1<<29);	//P1.25 Input (joysticks on PORT1 defined as Input) 
	
	// JOYSTICK LEFT
	LPC_PINCON->PINSEL3 &= ~(3<<22); // PIN mode GPIO (00b value per P1.27)
	LPC_GPIO1-> FIODIR &= ~(1<<27); // P1.27 Input (joystick on PORT1 defined as Input)
	
	// JOYTICK RIGHT
	LPC_PINCON->PINSEL3 &= ~(3<<24); // PIN Moode GPIO (00b value per P1.28)
	LPC_GPIO1->FIODIR &= ~(1<<28);  //  P1.28 Input (joystick on PORT1 defined as Input)
	
	// JOYSTICK DOWN
	LPC_PINCON->PINSEL3 &= ~(3<<20);  // PIN mode GPIO (00b value per P1.26)
	LPC_GPIO1->FIODIR &= ~(1<<26);    // P1.26 Input (joystick on PORT1 defined as Input)

	//	JOYSTICK SELECT
	LPC_PINCON->PINSEL3 &= ~(3<<18); // PIN mode GPI0 (00b value per P125)
	LPC_GPIO1->FIODIR &= ~(1<<25); // P1.29 Input (joystick on PORT1 defined as Input)
	
	
	
}
