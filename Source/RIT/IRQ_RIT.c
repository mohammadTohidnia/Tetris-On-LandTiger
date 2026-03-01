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
#include "My_Functions/my_func.h"
#include <stdbool.h>

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

void RIT_IRQHandler (void)
{
	static int downKEY1 = 0 ;
	static int downKEY2 = 0 ;
	
	if(((LPC_GPIO2->FIOPIN) & (1 << 11)) == 0){  // KEY1 is pressed
		downKEY1++;
		//reset_RIT();
		if(downKEY1 == 1){
			
			if(is_gameOver){
				reinitialize_game();
			}
			else{
	
				if(is_paused){ 
					// WE ARE RESUMING
					// 1. Draw Text FIRST (while Timers are still blocked by is_paused=true)
					GUI_Text(175, 15, (uint8_t *)"RESUMED", Black, White);
					// 2. Enable Timers
					is_paused = false;
				}
				else{
					// WE ARE PAUSING
					// 1. Block Timers FIRST
					is_paused = true;
					// 2. Draw Text (Safe now)
					GUI_Text(175, 15, (uint8_t *)"PAUSED ", Black, White);
				}
	
			}
			
		}
	}
	else{
		if(downKEY1 > 0){
			downKEY1 = 0 ;
			//disable_RIT();
			//reset_RIT();
			NVIC_EnableIRQ(EINT1_IRQn);
			
			LPC_PINCON->PINSEL4 |= (1 << 22) ;  // Set again the button to external interrupt
		}
		
	}
	
	
	
	if((LPC_GPIO2->FIOPIN & (1 << 12)) == 0){ // KEY2 Pressed
        downKEY2++;
        if(downKEY2 == 1){
            if(!is_paused && !is_gameOver){
                // CRITICAL: Disable Timers during HardDrop to prevent LCD corruption
                NVIC_DisableIRQ(TIMER0_IRQn);
                NVIC_DisableIRQ(TIMER1_IRQn);
                
                HardDrop(); 
                
                // Re-enable Timers
                NVIC_EnableIRQ(TIMER0_IRQn);
                NVIC_EnableIRQ(TIMER1_IRQn);
            }
        }
    }
    else{ // Released
        if(downKEY2 > 0){
            downKEY2 = 0;
            NVIC_EnableIRQ(EINT2_IRQn);
            LPC_PINCON->PINSEL4 |= (1 << 24); 
        }
    }
    
    if(downKEY1 == 0 && downKEY2 == 0){
        disable_RIT();
        reset_RIT();
    }
	
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
