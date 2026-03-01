#include "my_func.h"
#include "GLCD/GLCD.h"
#include <stdlib.h>

uint32_t score = 0;
uint32_t high_score = 0;
uint32_t line = 0;
volatile bool is_paused = false;
bool volatile is_gameOver = false;


// dimensions for each shape
volatile int left = 0;
volatile int right = 0;
volatile int up = 0;
volatile int down = 0;

// Current constants
uint8_t current_shapeID = 0;
uint8_t current_rotateINDEX = 0;
uint16_t leftCornerX[4] = {0, 0, 0, 0};
uint16_t leftCornerY[4] = {0, 0, 0, 0};

uint16_t blockX[4] = {0, 0, 0, 0};
uint16_t blockY[4] = {0, 0, 0, 0};

// Colors for each shape    I       O       T       J     L     S     Z
uint16_t shape_color[7] = {Cyan, Yellow, Magenta, Blue, Grey, Green, Red};
volatile uint16_t block_color;

field field_state[field_height][field_width]; // an array to store each shape when it is placed
// Data structure for shapes. We define coordinates for each shape in an array. This array has three dimensions:
//                                                                              7: We have 7 shapes
//                                                                              4: We have 4 possible rotations
//                                                                              4: We have 4 blocks

// A structure to store the x and y of each block (left corner)


point shapes[7][4][4] = {
                                  // Shape I Rotation States , Rotates are clockwise
	{	{{2,0}, {2,1}, {2,2}, {2,3}}, 
		{{0,2}, {1,2}, {2,2}, {3,2}},
		{{2,0}, {2,1}, {2,2}, {2,3}},
		{{0,2}, {1,2}, {2,2}, {3,2}},
	},
	
	                               // Shape O Rotation States (Shape O doesn't change when it rotates)
	{	{{0,0}, {0,1}, {1,1}, {1,0}}, 
		{{0,0}, {0,1}, {1,1}, {1,0}},
		{{0,0}, {0,1}, {1,1}, {1,0}},
		{{0,0}, {0,1}, {1,1}, {1,0}},
	},
	
		                               // Shape T Rotation States 
	{	{{0,0}, {1,0}, {2,0}, {1,1}}, 
		{{1,0}, {1,1}, {1,2}, {0,1}},
		{{1,0}, {0,1}, {1,1}, {2,1}},
		{{0,0}, {0,1}, {0,2}, {1,1}},
	},
	
			                               // Shape J Rotation States 
	{	{{1,0}, {1,1}, {1,2}, {0,2}}, 
		{{0,0}, {0,1}, {1,1}, {2,1}},
		{{0,0}, {0,1}, {0,2}, {1,0}},
		{{0,0}, {1,0}, {2,0}, {2,1}},
	},
	
				                               // Shape L Rotation States 
	{	{{0,0}, {0,1}, {0,2}, {1,2}}, 
		{{0,0}, {1,0}, {2,0}, {0,1}},
		{{0,0}, {1,0}, {1,1}, {1,2}},
		{{0,1}, {1,1}, {2,1}, {2,0}},
	},
	
				                               // Shape S Rotation States 
	{	{{0,1}, {1,1}, {1,0}, {2,0}}, 
		{{0,0}, {0,1}, {1,1}, {1,2}},
		{{0,1}, {1,1}, {1,0}, {2,0}},
		{{0,0}, {0,1}, {1,1}, {1,2}},
	},
	
					                               // Shape Z Rotation States 
	{	{{0,0}, {1,0}, {1,1}, {2,1}}, 
		{{1,0}, {1,1}, {0,1}, {0,2}},
		{{0,0}, {1,0}, {1,1}, {2,1}},
		{{1,0}, {1,1}, {0,1}, {0,2}},
	}
	
};

void generate_tetromino(void){

	current_shapeID = rand() % 7 ; // It picks one shape randomly (0 to 6)
	block_color = shape_color[current_shapeID];
	//current_rotateINDEX = 0; // Starts with initial rotation
	bool is_overlap = false;
	
	int i ;
	
	for (i=0 ; i<4 ; i++){
	
		blockX[i] = shapes[current_shapeID][current_rotateINDEX][i].x ;
		blockY[i] = shapes[current_shapeID][current_rotateINDEX][i].y ;
		
		// Before generating new shape, we check if there is another block existing in the place we want to generate
		// if the state is 1, it means that the new shape will overlap the prevoius one and the user looses.
		if(field_state[blockY[i]][blockX[i]].state == 1){ 
			is_overlap = true;
		}
		
		
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
	
	if(is_overlap){
		is_gameOver = true;
		
		GUI_Text(175, 15, (uint8_t *)"       ", Black, White);
		
		// Draw Game Over Text
		LCD_ClearArea(40, 110,100, 60, White);
		
		// Show the game over to the user
		GUI_Text(45, 120,(uint8_t *)"GAME OVER!", Red, White);
		GUI_Text(45, 145,(uint8_t *)"Press KEY1", Blue, White);
		
		// update the high score value
		if(score > high_score){
			high_score = score;
		}
	}
	
}




void LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t length, uint16_t width, uint16_t color){
	
	LCD_DrawLine(x0, y0, x0 + width, y0, color);
	LCD_DrawLine(x0 + width, y0, x1 , y1, color);
	LCD_DrawLine(x1 , y1, x0 , y0 + length, color);
	LCD_DrawLine(x0 , y0 + length, x0 , y0, color);
}

void playingField_init(void){
	//all_LED_off();
	LCD_Clear(Black);
	//LCD_DrawLine(20, 20, 30, 20, White);
	
	// Creating fields
	LCD_DrawRectangle(0, 0, 160, 310, 310, 160, White); // Border Field
	//LCD_DrawRectangle(5, 5, 155, 305, 300, 150, White); // Playing Field
	LCD_DrawRectangle(165, 0, 239, 310, 310, 74, White); // Score Field
	
	// Creating Information bar
	GUI_Text(190, 50,(uint8_t *)"TOP", White, Black);
	GUI_Text(185, 150,(uint8_t *)"SCORE", White, Black);
	GUI_Text(185, 250,(uint8_t *)"LINES", White, Black);
}

void startGame(void){
	
	char s_tmp[20];
	playingField_init();
	
	GUI_Text(175, 15, (uint8_t *)"RESUMED", Black, White);
	/*Resetting values to zero*/
	sprintf(s_tmp, "%d", high_score);
	GUI_Text(190, 65,(uint8_t *)(s_tmp), White, Black);
	
	sprintf(s_tmp, "%d", score);
	GUI_Text(185, 165,(uint8_t *)s_tmp, White, Black);
	
	sprintf(s_tmp, "%d", line);
	GUI_Text(185, 265,(uint8_t *)s_tmp, White, Black);
	
}


void LCD_ClearArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    uint16_t i, j;
    
    // Loop through every row of the area
    for (i = 0; i < h; i++)
    {
        // Set cursor to the start of the current row
        LCD_SetCursor(x, y + i); 
        
        // Prepare to write to GRAM (0x0022 is the RAM Write command)
        LCD_WriteIndex(0x0022); 
        
        // Fill the row
        for (j = 0; j < w; j++)
        {
            LCD_WriteData(color);
        }
    }
}


int findMin(uint16_t arr[], int size) {
    
    // Initialize min with the first element
    int min = arr[0];
		int i;
    // Iterate through the rest of the array
    for (i = 1; i < size; i++) {
        if (arr[i] < min) {
            min = arr[i];
        }
    }

    return min;
}

int findMax(uint16_t arr[], int size) {
    
    // Initialize min with the first element
    int max = arr[0];
		int i;
    // Iterate through the rest of the array
    for (i = 1; i < size; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }

    return max;
}


void check_lines(void){

	int i, j, k, h;
	int cleared_lines = 0;
	
	bool is_line_full =false;
	bool line_detected ;
	
	for(i = field_height - 1 ; i >= 0 ; i--){       // rows
		line_detected = true;                         // We assume that this line is full
		for(j = 0 ; j < field_width ; j++){           // columns                                                   
			if(field_state[i][j].state == 0){           // If there is any block in which the state is 0, so the line is not full
				line_detected = false ;
			}
		}
		
		if(line_detected){
			cleared_lines++;
			
			// The line is full. Clear the line
			LCD_ClearArea(0, i, field_width, block_size, Black);
			
			// Shift the above lines one block to the bottom
			for(k = i ; k > 0 ; k--){
				for(h = 0 ; h < field_width ; h++){
					field_state[k][h].state = field_state[k-1][h].state ;
					field_state[k][h].color = field_state[k-1][h].color ;
				}
			}
			
			// Clear the top of the field, because it is shifted down one block
			for(k = 0 ; k < field_width ; k++){
				field_state[0][k].state = 0;
				field_state[0][k].color = Black ;
			
			}
			
			
			
			i++;  // This line is very important and took a lot of my time to figure it out!!!!!!!!!
						// When one line is full, everything above is shifted down and then the for loop goes for the upper line.
			      // However, when the above line is shifte down, the index to check it shouldn't be updated. So, i put the i++
			      // istruction to remove the i-- by loop. In this way, if the above line is also full, it will be checked and deleted
		}
	}
	
	
	
	if(cleared_lines > 0){
		
		line += cleared_lines ;
		
		score += (cleared_lines * 100);
		
		if(cleared_lines == 4){
			score += 600 ;
		}
		
		redraw_field();
	
	}
	
	

}

void redraw_field(void){
	int k, h;
	// Draw the new field
	for(k = 0 ; k < field_height ; k++){
		for(h =0 ; h < field_width ; h++){
			
			uint16_t px = start_posX + (h * block_size);
			uint16_t py = start_posY + (k * block_size);
			
			if (field_state[k][h].state == 1){

				
				LCD_ClearArea(px + 1, py + 1, block_size - 2, block_size - 2, field_state[k][h].color);
				// Above Line
				LCD_DrawLine(px, py, px + block_size - 1, py, Black);
				// Bottom Line
				LCD_DrawLine(px, py + block_size - 1, px + block_size - 1, py + block_size - 1, Black);
				// Left Line
				LCD_DrawLine(px, py, px, py + block_size - 1, Black);
				// Right Line
				LCD_DrawLine(px + block_size - 1, py, px + block_size - 1, py + block_size - 1, Black);
				
				//LCD_DrawRectangle(px, py, px + block_size, py + block_size, block_size, block_size, field_state[k][h].color) ;

			}
			else{
				LCD_ClearArea(px, py, block_size, block_size, Black) ;
			}
		
		}
	
	}
	
	
	// update score panel
	update_score_panel();

}

void update_score_panel(void){
	char s_tmp[20] ;
	
	sprintf(s_tmp, "%d", score);
	GUI_Text(185, 165,(uint8_t *)s_tmp, White, Black);
	
	sprintf(s_tmp, "%d", line);
	GUI_Text(185, 265,(uint8_t *)s_tmp, White, Black);
}

void reinitialize_game(void){
	
	
	score = 0;
	line = 0;
	
	// reinitialize field_state
	int i, j;
	for(i = 0 ; i < field_height ; i++){
		for(j = 0 ; j < field_width ; j++){
			field_state[i][j].state = 0;
			field_state[i][j].color = Black ;
		}
	}
	
	// clear the playing field and reinitialize the game
	LCD_Clear(Black);
	startGame();
	generate_tetromino();
	
	is_gameOver = false;
	is_paused = false;
}


void HardDrop(void){
    int i, j, min_dist = 20; 
    int dist;

    // Calculate drop distance
    for(i = 0; i < 4; i++){
        dist = 0;
        int y_check = blockY[i] + 1;
        // Check for floor (20) AND existing blocks
        while(y_check < field_height && field_state[y_check][blockX[i]].state == 0){
            dist++;
            y_check++;
        }
        if(dist < min_dist){
					min_dist = dist;
				} 
    }

    // Clear Old Position
    for(j = 0; j < 4; j++){
        LCD_ClearArea(leftCornerX[j], leftCornerY[j], block_size, block_size, Black);
    }

    // Update Position and Draw and update the playing field state 
    for(i = 0; i < 4; i++){
        blockY[i] = blockY[i] + min_dist;
        leftCornerX[i] = start_posX + (blockX[i] * block_size);
        leftCornerY[i] = start_posY + (blockY[i] * block_size);
        
        // Draw the block 
        LCD_ClearArea(leftCornerX[i] + 1, leftCornerY[i] + 1, block_size - 2, block_size - 2, block_color);
        LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i], Black);
        LCD_DrawLine(leftCornerX[i], leftCornerY[i] + block_size - 1, leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
        LCD_DrawLine(leftCornerX[i], leftCornerY[i], leftCornerX[i], leftCornerY[i] + block_size - 1, Black);
        LCD_DrawLine(leftCornerX[i] + block_size - 1, leftCornerY[i], leftCornerX[i] + block_size - 1, leftCornerY[i] + block_size - 1, Black);
        
        // Update state
        field_state[blockY[i]][blockX[i]].state = 1;
        field_state[blockY[i]][blockX[i]].color = block_color;
    }
    
    
    score += 10; 
    update_score_panel();
    check_lines();
    generate_tetromino();
}