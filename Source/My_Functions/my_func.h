#include <stdio.h>
#include <GLCD/GLCD.h>
#include <TouchPanel/TouchPanel.h>
#include <stdbool.h>


extern uint32_t score, high_score, line;
extern volatile bool is_paused;
extern volatile bool is_gameOver ;

// Game constants
#define start_posX 5
#define start_posY 5
#define block_size 15
#define field_width 10
#define field_height 20

typedef struct{
	uint16_t x;
	uint16_t y;
} point;

typedef struct {
    uint8_t state;   // 0 or 1 (0 means the block is empty, 1 means it's full of a block)
    uint16_t color;  // The specific color for each block
} field;


extern volatile int left;
extern volatile int right;
extern volatile int up;
extern volatile int down;

// Current constants
extern uint8_t current_shapeID ;
extern uint8_t current_rotateINDEX ;
extern uint16_t leftCornerX[4] ;
extern uint16_t leftCornerY[4] ;
extern uint16_t blockX[4] ;
extern uint16_t blockY[4] ;
extern volatile uint16_t block_color ;
//extern uint8_t bottom_state[10];
extern point shapes[7][4][4];
extern field field_state[20][10];

void LCD_DrawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t length, uint16_t width, uint16_t color);
void playingField_init(void);
void startGame(void);
void generate_tetromino(void);
void LCD_ClearArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void check_lines(void);
int findMax(uint16_t arr[], int size);
int findMin(uint16_t arr[], int size);
void redraw_field(void);
extern void update_score_panel(void);
extern void reinitialize_game(void);
extern void HardDrop(void);