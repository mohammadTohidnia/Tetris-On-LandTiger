#include "button.h"
#include "LPC17xx.h"
#include "My_Functions/my_func.h"
#include "GLCD/GLCD.h"

#include "../led/led.h"
#include "../timer/timer.h"
#include "RIT.h"

void EINT0_IRQHandler (void)	  	/* INT0														 */
{
	LED_On(0);
	LPC_SC->EXTINT |= (1 << 0);     /* clear pending interrupt         */
}


void EINT1_IRQHandler (void)	  	/* KEY1														 */
{

	enable_RIT();                          // enable timer to count every 50 msec
	NVIC_SetPriority(RIT_IRQn, 0);
	NVIC_DisableIRQ(EINT1_IRQn);           // disable interrupt
	LPC_PINCON->PINSEL4    &= ~(1 << 22);  // set the button to GPIO
	
	LPC_SC->EXTINT |= (1 << 1);     /* clear pending interrupt         */
}

void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
	
	enable_RIT();               // Start Debouncing
	NVIC_SetPriority(RIT_IRQn, 0);
	NVIC_DisableIRQ(EINT2_IRQn); // Stop listening to this specific interrupt
    
  LPC_PINCON->PINSEL4 &= ~(1 << 24);
    
	
	
  LPC_SC->EXTINT |= (1 << 2);     /* clear pending interrupt         */  
	  
}


