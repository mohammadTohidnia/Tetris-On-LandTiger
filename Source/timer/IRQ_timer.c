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
#include "led.h"
#include "My_Functions/my_func.h"
#include <stdbool.h>


/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/


void TIMER0_IRQHandler (void)
{
	
	// At first, check if the game is over or paused. We block timers not to shift any shape until the user press KEY1
	if(is_gameOver || is_paused){
		LPC_TIM0->IR = 1 ;
		return ;
	}
	
	int i , j;
	static int count = 0;
	//bool is_reached = false;
	bool is_placed = false;
	
	count++;
	
	// Check when the shape has to be placed
	for(i = 0 ; i <4 ; i++){
		 
		if(blockY[i] >= 19){  // The shape reached the bottom field
			is_placed = true;
			break;
		}
		
		if(field_state[blockY[i] + 1][blockX[i]].state == 1){ // The bottom of the shape is full. So, the shape has to be placed
		
			is_placed = true;
			break;
		}
	}
	
	// Save the new state of the playing field
	if(is_placed){ 
		
		for(i = 0 ; i < 4 ; i++){
			field_state[blockY[i]][blockX[i]].state = 1;
			field_state[blockY[i]][blockX[i]].color = block_color;
		}
		
		// Calculate score
		score += 10; // one placement is 10 points
		update_score_panel();
		
		// Check lines for clearing
		check_lines();
	
		// check for game over in generate_tetromino() function before generating new shapr
		generate_tetromino();
		is_placed = false;
		
	}
	else{ // if the shape is not placed, shift it one square to the bottom
		for(j = 0 ; j < 4 ; j++){
			LCD_ClearArea(leftCornerX[j], leftCornerY[j] , block_size , block_size, Black);
		}
	
		for(i = 0 ; i < 4 ; i++){
		
			blockY[i] = blockY[i] + 1; 
			leftCornerX[i] = start_posX + (blockX[i] * block_size) ;
			leftCornerY[i] = start_posY + (blockY[i] * block_size) ;
			
			LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, block_size - 2, block_size - 2, block_color);
			// Above Line
			LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i], Black);
			// Bottom Line
			LCD_DrawLine(leftCornerX[i], leftCornerY[i] + block_size - 1, leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
			// Left Line
			LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i], leftCornerY[i] + block_size - 1, Black);
			// Right Line
			LCD_DrawLine(leftCornerX[i] + block_size - 1, leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
				
			//LCD_DrawRectangle(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size, leftCornerY[i] + block_size, block_size, block_size, block_color);
			//LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, leftCornerX[i] + block_size - 2, leftCornerY[i] + block_size - 2, block_color);
		
		}
		
	}	
	
	LPC_TIM0->IR = 1;			/* clear interrupt flag */
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
	
	// At first, check if the game is over or paused. We block timers not to shift any shape until the user press KEY1
	if(is_gameOver || is_paused){
		LPC_TIM1->IR = 1 ;
		return ;
	}
	
	static int select = 0;
	static int select2 = 0;
	static int select3 = 0;
	static int select4 = 0;
	int i, j;
	
	
	
	if(((LPC_GPIO1->FIOPIN) & (1 << 27)) ==  0){ // Joystick left is pressed
		select++;
		bool can_turn_left = true;
		for (i = 0; i<4 ; i++){  // Check if the shape can turn left
			
			if((field_state[blockY[i]][blockX[i] - 1].state == 1) || blockX[i] == 0){
				can_turn_left = false ;
			}
		}
		
		if(select == 1){
			
			if(can_turn_left){
			
				for(j = 0 ; j < 4 ; j++){
					LCD_ClearArea(leftCornerX[j], leftCornerY[j] , block_size , block_size , Black);
				}
	
				for(i = 0 ; i < 4 ; i++){
					
					blockX[i] = blockX[i] - 1; 
					leftCornerX[i] = start_posX + (blockX[i] * block_size) ;
					leftCornerY[i] = start_posY + (blockY[i] * block_size) ;
					
					LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, block_size - 2, block_size - 2, block_color);
					// Above Line
					LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i], Black);
					// Bottom Line
					LCD_DrawLine(leftCornerX[i], leftCornerY[i] + block_size - 1, leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
					// Left Line
					LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i], leftCornerY[i] + block_size - 1, Black);
					// Right Line
					LCD_DrawLine(leftCornerX[i] + block_size - 1, leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
								
					
					//LCD_DrawRectangle(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size, leftCornerY[i] + block_size, block_size, block_size, block_color);
					//LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, leftCornerX[i] + block_size - 2, leftCornerY[i] + block_size - 2, block_color);
						
				}	
			}
		}
	}
	else{
		select = 0;
	}
	
	
	
	if(((LPC_GPIO1->FIOPIN) & (1 << 28)) ==  0){ // Joystick right is pressed
		select2++;
		bool can_turn_right = true;
		for (i = 0; i<4 ; i++){  // Check if the shape can turn right
			
			if((field_state[blockY[i]][blockX[i] + 1].state == 1) || blockX[i] == 9){
				can_turn_right = false ;
			}
		}
		
		if(select2 == 1){
			
			if(can_turn_right){
			
				for(j = 0 ; j < 4 ; j++){
					LCD_ClearArea(leftCornerX[j], leftCornerY[j] , block_size , block_size , Black);
				}
	
				for(i = 0 ; i < 4 ; i++){
					
					blockX[i] = blockX[i] + 1; 
					leftCornerX[i] = start_posX + (blockX[i] * block_size) ;
					leftCornerY[i] = start_posY + (blockY[i] * block_size) ;
					
					LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, block_size - 2, block_size - 2, block_color);
					// Above Line
					LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i], Black);
					// Bottom Line
					LCD_DrawLine(leftCornerX[i], leftCornerY[i] + block_size - 1, leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
					// Left Line
					LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i], leftCornerY[i] + block_size - 1, Black);
					// Right Line
					LCD_DrawLine(leftCornerX[i] + block_size - 1, leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
								
								
					
					//LCD_DrawRectangle(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size, leftCornerY[i] + block_size, block_size, block_size, block_color);
					//LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, leftCornerX[i] + block_size - 2, leftCornerY[i] + block_size - 2, block_color);
						
				}	
			}
		}
	}
	else{
		select2 = 0;
	}
	
	
	if(((LPC_GPIO1->FIOPIN) & (1 << 29)) ==  0){ // Joystick UP is pressed. We have to rotate shapes
		select3++;
		int DeltaX[4];
		int DeltaY[4];
		int k;
		
		if(select3 == 1){
			
			int prevoius_rotation = current_rotateINDEX; // save the old rotation index
			current_rotateINDEX++;
			
			if(current_rotateINDEX > 3){
				current_rotateINDEX = 0;
			}
			
			for(k = 0 ; k < 4 ; k++){
			
				DeltaX[k] = (int)blockX[k] - shapes[current_shapeID][prevoius_rotation][k].x ;
				DeltaY[k] = (int)blockY[k] - shapes[current_shapeID][prevoius_rotation][k].y ;
				
			}
			
			for(j = 0 ; j < 4 ; j++){
				LCD_ClearArea(leftCornerX[j], leftCornerY[j] , block_size , block_size , Black);
			}
		
			for(i = 0 ; i < 4 ; i++){
				
				blockX[i] = DeltaX[i] + shapes[current_shapeID][current_rotateINDEX][i].x ;
				blockY[i] = DeltaY[i] + shapes[current_shapeID][current_rotateINDEX][i].y ;
					
				leftCornerX[i] = start_posX + (blockX[i] * block_size) ;
				leftCornerY[i] = start_posY + (blockY[i] * block_size) ;
				
				LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, block_size - 2, block_size - 2, block_color);
				// Above Line
				LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i], Black);
				// Bottom Line
				LCD_DrawLine(leftCornerX[i], leftCornerY[i] + block_size - 1, leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
				// Left Line
				LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i], leftCornerY[i] + block_size - 1, Black);
				// Right Line
				LCD_DrawLine(leftCornerX[i] + block_size - 1, leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
						
				
				//LCD_DrawRectangle(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size, leftCornerY[i] + block_size, block_size, block_size, block_color);
			}
			
		}
		
		
		
	}
	else{
		select3 = 0;
	}
	
	
	if(((LPC_GPIO1->FIOPIN) & (1 << 26)) ==  0){ // The joystick down is holding
		
		LPC_TIM0->MR0 = 0xBEBC20; // set timer to 500 ms (2 squares per second)
	}
	else{
		LPC_TIM0->MR0 = 0x17D7840; // set timer to 1 s (1 squares per second)
	}
	
	
  LPC_TIM1->IR = 0x3F;			/* clear interrupt flag */
  return;

}




/******************************************************************************
**                            End Of File
******************************************************************************/
