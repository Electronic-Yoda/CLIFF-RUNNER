/* This files provides address values that exist in the system */
#define BOARD                 "DE1-SoC"

/* Memory */
#define DDR_BASE              0x00000000
#define DDR_END               0x3FFFFFFF
#define A9_ONCHIP_BASE        0xFFFF0000
#define A9_ONCHIP_END         0xFFFFFFFF
#define SDRAM_BASE            0xC0000000
#define SDRAM_END             0xC3FFFFFF
#define FPGA_ONCHIP_BASE      0xC8000000
#define FPGA_ONCHIP_END       0xC803FFFF
#define FPGA_CHAR_BASE        0xC9000000
#define FPGA_CHAR_END         0xC9001FFF

/* Cyclone V FPGA devices */
#define LEDR_BASE             0xFF200000
#define HEX3_HEX0_BASE        0xFF200020
#define HEX5_HEX4_BASE        0xFF200030
#define SW_BASE               0xFF200040
#define KEY_BASE              0xFF200050
#define JP1_BASE              0xFF200060
#define JP2_BASE              0xFF200070
#define PS2_BASE              0xFF200100
#define PS2_DUAL_BASE         0xFF200108
#define JTAG_UART_BASE        0xFF201000
#define JTAG_UART_2_BASE      0xFF201008
#define IrDA_BASE             0xFF201020
#define TIMER_BASE            0xFF202000
#define AV_CONFIG_BASE        0xFF203000
#define PIXEL_BUF_CTRL_BASE   0xFF203020
#define CHAR_BUF_CTRL_BASE    0xFF203030
#define AUDIO_BASE            0xFF203040
#define VIDEO_IN_BASE         0xFF203060
#define ADC_BASE              0xFF204000

/* Cyclone V HPS devices */
#define HPS_GPIO1_BASE        0xFF709000
#define HPS_TIMER0_BASE       0xFFC08000
#define HPS_TIMER1_BASE       0xFFC09000
#define HPS_TIMER2_BASE       0xFFD00000
#define HPS_TIMER3_BASE       0xFFD01000
#define FPGA_BRIDGE           0xFFD0501C

/* ARM A9 MPCORE devices */
#define   PERIPH_BASE         0xFFFEC000    // base address of peripheral devices
#define   MPCORE_PRIV_TIMER   0xFFFEC600    // PERIPH_BASE + 0x0600

/* Interrupt controller (GIC) CPU interface(s) */
#define MPCORE_GIC_CPUIF      0xFFFEC100    // PERIPH_BASE + 0x100
#define ICCICR                0x00          // offset to CPU interface control reg
#define ICCPMR                0x04          // offset to interrupt priority mask reg
#define ICCIAR                0x0C          // offset to interrupt acknowledge reg
#define ICCEOIR               0x10          // offset to end of interrupt reg
/* Interrupt controller (GIC) distributor interface(s) */
#define MPCORE_GIC_DIST       0xFFFED000    // PERIPH_BASE + 0x1000
#define ICDDCR                0x00          // offset to distributor control reg
#define ICDISER               0x100         // offset to interrupt set-enable regs
#define ICDICER               0x180         // offset to interrupt clear-enable regs
#define ICDIPTR               0x800         // offset to interrupt processor targets regs
#define ICDICFR               0xC00         // offset to interrupt configuration regs

/* VGA colors */
#define BLACK 0x0000
#define WHITE 0xFFFF
#define YELLOW 0xFFE0
#define RED 0xF800
#define GREEN 0x07E0
#define BLUE 0x001F
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define GREY 0xC618
#define PINK 0xFC18
#define ORANGE 0xFC00

#define BOX_LEN 10
	#define NUM_BOXES 8

#define ABS(x) (((x) > 0) ? (x) : -(x))

/* Screen size. */
#define RESOLUTION_X 320
#define RESOLUTION_Y 240
	
//define character codes for VGA seven segment
#define S {1,0,1,1,0,1,1}
	
#include <stdlib.h>
#include <time.h>	
#include <stdio.h>
#include <stdbool.h>

/* Globals */
bool menuOn = false;
bool gameOn = false;
bool helpOn = false;
bool gameOver = false;
bool endProgram = false;
bool movRight = false;
volatile int pixel_buffer_start; // global variable
volatile char* character_buffer = (char*) 0xC9000000;
/*	Example of using character buffer
	int x = 30;
	int y = 2;
		char Array[] = "CLIFF RUNNER";
	for (int i = 0; i < sizeof(Array); i++) {
		*(char *) (character_buffer + (y << 7) + x) = Array[i];
		x++; /*
	
/* Ninja Movement */
int xloc = 30;
int yloc = 190;
int xSz = 19;
int ySz = 19;
int dx = 0;
bool shifting = false;

/* Obstacle Movement*/
int xObs = 30;
int yObs = 0;
int xObsSz = 80;
int yObsSz = 10;
int obsDy = 7;
bool generated = false;

/* Item drop Movement */
int xItem = 154;
int yItem = 30;
int xItemSz = 10;
int yItemSz = 10;
int itemDy = 10;
bool itemDrop = false;
bool itemHit = false;
bool itemEffect = false;

/* Object attributes */
int side = 0;
int type = 0;
int width = 0;
bool onLeft = true;
bool obsHit = false;

/* Score and gameloop attributes */
int loopCounter = 0;
int score = 0;
int highScore = 0;
int effectCounter = 0;
int firstTime = true;

const int seven_seg_digits_decode[75]= {
/*  0     1     2     3     4     5     6     7     8     9     :     ;     */
    0x7E, 0x30, 0x6D, 0x79, 0x33, 0x5B, 0x5F, 0x70, 0x7F, 0x7B, 0x00, 0x00, 
/*  <     =     >     ?     @     A     B     C     D     E     F     G     */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 
/*  H     I     J     K     L     M     N     O     P     Q     R     S     */
    0x37, 0x06, 0x3C, 0x57, 0x0E, 0x55, 0x76, 0x7E, 0x67, 0x73, 0x46, 0x5B, 
/*  T     U     V     W     X     Y     Z     [     \     ]     ^     _     */
    0x0F, 0x3E, 0x3E, 0x5C, 0x13, 0x3B, 0x6D, 0x00, 0x00, 0x00, 0x00, 0x00, 
/*  `     a     b     c     d     e     f     g     h     i     j     k     */
    0x00, 0x77, 0x1F, 0x4E, 0x3D, 0x4F, 0x47, 0x5E, 0x37, 0x06, 0x3C, 0x57, 
/*  l     m     n     o     p     q     r     s     t     u     v     w     */
    0x0E, 0x55, 0x15, 0x1D, 0x67, 0x73, 0x05, 0x5B, 0x0F, 0x3E, 0x1C, 0x5C, 
/*  x     y     z     */
    0x13, 0x3B, 0x6D
};
	
/* function prototypes */
void HEX_PS2(char, char, char);
void clearFIFO();
void pickStateAndReact();
void DrawMenuAndReact();
void drawGameAndReact();
void drawHelpAndReact();
void drawGameOverAndReact();
void drawA7Segment(int *segCode, int xmin, int ymin, int segLength, short int color);	
void decToBinary7Bits(int n, int *correctOrder);
void drawString(char *str, int xmin, int ymin, int segLength, short int color);
bool hitDetected();
void resetVariables();
bool itemHitDetected();
void clear_obj();
void clear_screen_except_border(int xMin, int yMin, int xMax, int yMax);

void plot_pixel(int x, int y, short int line_color);	
void clear_screen();
void draw_line(int x0, int y0, int x1, int y1, short int color);	
void swap(int * a, int * b);
void wait_for_vsync();
void fillRectangle(int x, int y, int xSize, int ySize, short int color);
void fillRectangleNew(int x, int y, int xSize, int ySize, short int color);

/*******************************************************************************
* This program demonstrates use of the PS/2 port by displaying the last three
* bytes of data received from the PS/2 port on the HEX displays.
******************************************************************************/
int main(void) {
	
	//first set menuOn to true since we want program to open with menu
	menuOn = true;
	
	//while loop breaks when endProgram == true
	while(!endProgram) {
		pickStateAndReact();
	}
	
	//clear screen as program ends
	volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
	
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	
    clear_screen();
}

void pickStateAndReact() {
	
	//Selects states for which screen to display
    if (menuOn) {
        DrawMenuAndReact();
    }
    else if (gameOn) {
        drawGameAndReact();
    }
    else if (helpOn) {
        drawHelpAndReact();
        
    }else if (gameOver) {
        drawGameOverAndReact();
    }
}

void DrawMenuAndReact() {
    clearFIFO();
    printf("Draw Menu. Press Enter to Play. Press ESE to quit program\n");

	
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
    
    clear_screen();
    
    //drawA7Segment(segCode, 20, 20, segLength, WHITE);
	//draw menu text
    char gameName[] = "CLIFF RUnnER";
    drawString(gameName, 19, 21, 15, RED);
	drawString(gameName, 21, 19, 15, CYAN);
	drawString(gameName, 22, 18, 15, WHITE);
    
    drawString("PLAY", 100, 80, 10, GREEN);
    fillRectangleNew(185, 85, 40, 17, GREEN);
    drawString("ENTER", 190, 90, 3, BLACK);
    
    drawString("HELP", 100, 120, 10, GREY);
    fillRectangleNew(185, 125, 40, 17, GREY);
    drawString("SHIFT", 190, 130, 3, BLACK);
    
    drawString("QUIT", 100, 160, 10, GREY);
    fillRectangleNew(185, 165, 40, 17, GREY);
    drawString("ESC", 190, 170, 3, BLACK);
    
    fillRectangleNew(0, 70, 75, RESOLUTION_Y-70, BLUE);
    fillRectangleNew(245, 70, RESOLUTION_X-245, RESOLUTION_Y-70, PINK);
    draw_line(75, 70, 75, RESOLUTION_Y, GREY);
     draw_line(0, 70, 75, 70, GREY);
	draw_line(245, 70, 245, RESOLUTION_Y, WHITE);
   
    draw_line(245, 70, RESOLUTION_X, 70, WHITE);
    
    /* Declare volatile pointers to I/O registers (volatile means that IO load
    and store instructions will be used to access these pointer locations,
    instead of regular memory loads and stores) */
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data, RVALID;
    char byte1 = 0, byte2 = 0, byte3 = 0;
	
    // PS/2 mouse needs to be reset (must be already plugged in)
    *(PS2_ptr) = 0xFF; // reset
    while (1) {
        PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000; // extract the RVALID field
        if (RVALID) {
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;
            HEX_PS2(byte1, byte2, byte3);
            if ((byte1 == (char)0x5A) && (byte2 == (char)0xF0) && (byte3 == (char)0x5A)) {
                printf("Enter clicked. Proceed to Game State\n");
                menuOn = false;
                gameOver = false;
                gameOn = true;
                return;
            }
            else if ((byte1 == (char)0x76) && (byte2 == (char)0xF0) && (byte3 == (char)0x76)) {
                printf("ESC clicked. Exist program\n");
                endProgram = true;
                return;
            }
            else if ((byte1 == (char)0x12) && (byte2 == (char)0xF0) && (byte3 == (char)0x12)) {
                printf("SHIFT clicked. Show Help Screen\n");
                menuOn = false;
                helpOn = true;
                return;
            }
        }
    }
}


void drawGameAndReact() {
    bool clearSuper = false;
	
	clearFIFO();
    printf("Draw Game. Press SPACE to switch directions. Press ESE to go back to Menu\n");
    
    /* Declare volatile pointers to I/O registers (volatile means that IO load
    and store instructions will be used to access these pointer locations,
    instead of regular memory loads and stores) */
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data, RVALID;
    char byte1 = 0, byte2 = 0, byte3 = 0;
	
    // PS/2 mouse needs to be reset (must be already plugged in)
    *(PS2_ptr) = 0xFF; // reset
     volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
	
    // declare other variables(not shown)
    // initialize location and direction of rectangles(not shown)
    /* set front pixel buffer to start of FPGA On-chip memory */
    *(pixel_ctrl_ptr + 1) = 0xC8000000; // first store the address in the
                                        // back buffer
    /* now, swap the front/back buffers, to set the front buffer location */
    wait_for_vsync();
	
    /* initialize a pointer to the pixel buffer, used by drawing functions */
    pixel_buffer_start = *pixel_ctrl_ptr;
    clear_screen(); 
	
		/* Drawing side borders in front buffer*/
		fillRectangle(0, 0, 29, RESOLUTION_Y, WHITE);
        draw_line(29, 0, 29, 239, CYAN);
		fillRectangle(289, 0, RESOLUTION_X - 289, RESOLUTION_Y, WHITE);
        draw_line(289, 0, 289, 239, CYAN);
	
		/* Drawing Scoreboard and banner in front buffer */
		fillRectangle(29, 0, 290, 30, WHITE);
		drawString("SCORE", 60, 10, 6, BLACK);
	
	// pixel_buffer_start points to the pixel buffer
    /* set back pixel buffer to start of SDRAM memory */
    *(pixel_ctrl_ptr + 1) = 0xC0000000;
    pixel_buffer_start = *(pixel_ctrl_ptr + 1); // we draw on the back buffer
	
		/* Drawing side borders in back buffer*/
		fillRectangle(0, 0, 29, RESOLUTION_Y, WHITE);
        draw_line(29, 0, 29, 239, CYAN);
		fillRectangle(289, 0, RESOLUTION_X - 289, RESOLUTION_Y, WHITE);
        draw_line(289, 0, 289, 239, CYAN);
	
		/* Drawing Scoreboard and banner in back buffer*/
		fillRectangle(29, 0, 290, 30, WHITE);
		drawString("SCORE", 60, 10, 6, BLACK);
    
    while (1)
    {
		loopCounter++;
		
		//score counter
		if(loopCounter % 5 == 0){
			score++;
			printf("score is: %d\n", score);
		}
		
		//drops items at random
		if(!itemDrop && (loopCounter % 2 == 0) && !itemEffect){
			if(rand() % 40 + 1 == 1){
				itemDrop = true;
				printf("highScore is: %d\n", highScore);
			}
		}
		
        /* Erase any boxes and lines that were drawn in the last iteration */
        clear_screen_except_border(30, 30 , RESOLUTION_X - 31, RESOLUTION_Y);
		
		// Clear the score by making 7 segments all white
		drawString("8888", 140, 10, 6, WHITE);
		
		//clears super text if necessary
		if (clearSuper) {
			drawString("88888", 220, 10, 6, WHITE);
			clearSuper = false;
		}
			
		
		
		// detects if ninja hit obstacles
        obsHit = hitDetected();
        if(!firstTime && obsHit && !itemEffect){
            menuOn = false;
            gameOn = false;
            helpOn = false;
            gameOver = true;
			if(score > highScore){
				highScore = score;
			}
            //resetVariables();
        	return;
         
        }
        obsHit = false;
		
		// detects if ninja collects items and gives effect status
		if(!firstTime && itemDrop && !itemEffect){
			itemHit = itemHitDetected();
			if(itemHit){
				yItem = 30;
				itemEffect = true;
				itemDrop = false;
			}
		}
		itemHit = false;
		
		// gives ninja immortality a countdown
		if(itemEffect){
			effectCounter++;
			if(effectCounter % 40 == 0){
				effectCounter = 0;
				itemEffect = false;
				// Clear SUPER 
				drawString("88888", 220, 10, 6, WHITE);
				clearSuper = true;
			}
		}
		
        /* Ninja movement */
        if(shifting && !onLeft){
            if(xloc < 259){
                xloc = xloc + dx;
            }
            else if (xloc >= 259){
                dx = 0;
                shifting = false;
            }
        }else if(shifting && onLeft){
            if(xloc > 40){
                xloc = xloc + dx;
            }
            else if (xloc <= 40){
                dx = 0;
                shifting = false;
            }
        }
        
        
		
        /* Obstacle movement */
		if(loopCounter % 50 == 0 && obsDy < 50){
			obsDy++;
		}
		
		/* Obstacle generation */
        if(!generated){
            
			side = rand() % 2 + 1; //1 - left side, 2 - right side
			type = rand() % 3 + 1;
			width = rand() % 3 + 1;
			
			if(width == 1){
				yObsSz = 10;
			}
			else if(width == 2){
				yObsSz = 13;
			}
			else if(width == 3){
				yObsSz = 16;
			}
			
            if(side == 1 && type == 1){
                xObs = 30;
				xObsSz = 80;
            }
			else if (side == 1 && type == 2){
				xObs = 30;
				xObsSz = 60;
			}
			else if (side == 1 && type == 3){
				xObs = 30;
				xObsSz = 40;
			}
            else if (side == 2 && type == 1){
                xObs = 209;
				xObsSz = 80;
            }
			else if (side == 2 && type == 2){
                xObs = 229;
				xObsSz = 60;
            }
			else if (side == 2 && type == 3){
                xObs = 249;
				xObsSz = 40;
            }
            yObs = 30;
            generated = true;
        }
        else if(generated){
            yObs = yObs + obsDy;
        }
        if(yObs >= 219){
            generated = false;
            side = 0;
        }
		
		if(itemDrop && !itemEffect){
			if(yItem >= 219){
				yItem = 30;
				itemDrop = false;
			}
			else{
				yItem = yItem + itemDy;
				fillRectangle(xItem, yItem, xItemSz, yItemSz, GREEN);
			}
		}
		
        /* Drawing obstacles */
		if(type == 1){
        	fillRectangle(xObs, yObs, xObsSz, yObsSz, RED);
		}else if(type == 2){
			fillRectangle(xObs, yObs, xObsSz, yObsSz, WHITE);
		}else if(type == 3){
			fillRectangle(xObs, yObs, xObsSz, yObsSz, CYAN);
		}
		
		//draw score
		char scoreAry[20];
		snprintf(scoreAry, 20, "%d", score);
		drawString(scoreAry, 140, 10, 6, RED);
		
		/* Drawing ninja */
		if(itemEffect){
        	fillRectangle(xloc, yloc, xSz, ySz, MAGENTA);
			drawString("SUPER", 220, 10, 6, MAGENTA);
		}
		else if(!itemEffect){
			fillRectangle(xloc, yloc, xSz, ySz, WHITE);
		}
		
        wait_for_vsync(); // swap front and back buffers on VGA vertical sync
        pixel_buffer_start = *(pixel_ctrl_ptr + 1); // new back buffer
        
        //the following reacts to keyboard press
        PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000; // extract the RVALID field
        if (RVALID) {
			
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;
            HEX_PS2(byte1, byte2, byte3);
            if ((byte1 == (char)0x29) && (byte2 == (char)0xF0) && (byte3 == (char)0x29) && !shifting) {
                printf("SpaceBar clicked. Change Direction!\n");
                if(onLeft){
                    dx = 40;
                    shifting = true;
                    onLeft = false;
                }else if (!onLeft){
                    dx = -40;
                    shifting = true;
                    onLeft = true;
                }
            }
            else if ((byte1 == (char)0x76) && (byte2 == (char)0xF0) && (byte3 == (char)0x76) && !shifting) {
                printf("ESC clicked. Back to Menu!\n");
                menuOn = true;
                gameOn = false;
                return;
            }
        }
        firstTime = false;
    }
}

bool itemHitDetected(){
	
	//detects if an item is collected
	if( ((xloc + xSz) >= xItem && xloc <= (xItem + xItemSz)) && ((yloc + ySz) >= yItem && yloc <= (yItem + yItemSz))){
		return true;
	}
	else{
		return false;
	}
}

bool hitDetected(){
	
	//detects which side is displayed and the conditions to detect collisions
	if(side == 1){
		if((xloc <= xObs + xObsSz) && ((yloc + ySz >= yObs + yObsSz) && (yloc <= yObs))){
			return true;
		}else if((xloc <= xObs + xObsSz) && ((yloc >= yObs ) && (yloc <= (yObs + yObsSz)))){
			return true;
		}
		else{
			return false;
		}
	}
	else if(side == 2){
		if((xloc + xSz >= xObs) && ((yloc + ySz >= yObs + yObsSz) && (yloc <= yObs))){
			return true;
		}else if((xloc + xSz >= xObs) && ((yloc >= yObs ) && (yloc <= (yObs + yObsSz)))){
			return true;
		}
		else{
			return false;
		}
	}
}

void drawHelpAndReact() {
    clearFIFO();
    printf("Draw Help. Press ESE to go back to Menu program\n");
    
    volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
    
    clear_screen();
	
	//Help menu text
        char gameName[] = "CLIFF RUnnER";
    drawString(gameName, 19, 21, 15, RED);
	drawString(gameName, 21, 19, 15, CYAN);
	drawString(gameName, 22, 18, 15, WHITE);
    
    drawString("How to play", 70, 65, 10, GREEN);
	fillRectangleNew(52, 100, 50, 20, GREY);
    drawString("Hit SPACE TO AVOID OBSTACLES", 20, 105, 5, GREEN);
	drawString("    SPACE", 20, 105, 5, BLACK);
    drawString("Collect green reward for SUPER", 25, 132, 5, GREY);
	drawString("        green reward", 25, 132, 5, GREEN);
	drawString("                         SUPER", 25, 132, 5, MAGENTA);
    
    drawString("MENU", 100, 160, 10, RED);
    fillRectangleNew(185, 165, 40, 17, GREY);
    drawString("ESC", 190, 170, 3, BLACK);
    
    /* Declare volatile pointers to I/O registers (volatile means that IO load
    and store instructions will be used to access these pointer locations,
    instead of regular memory loads and stores) */
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data, RVALID;
    char byte1 = 0, byte2 = 0, byte3 = 0;
    // PS/2 mouse needs to be reset (must be already plugged in)
    *(PS2_ptr) = 0xFF; // reset
    while (1) {
        PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
        RVALID = PS2_data & 0x8000; // extract the RVALID field
        if (RVALID) {
            /* shift the next data byte into the display */
            byte1 = byte2;
            byte2 = byte3;
            byte3 = PS2_data & 0xFF;
            HEX_PS2(byte1, byte2, byte3);
            
            if ((byte1 == (char)0x76) && (byte2 == (char)0xF0) && (byte3 == (char)0x76)) {
                printf("ESC clicked. Back to Menu\n");
                menuOn = true;
                helpOn = false;
                return;
            }
        }
    }
}

void drawGameOverAndReact() {
	clearFIFO();
	printf("Game Over. Press ESE to go back to Menu program\n");
	
	volatile int * pixel_ctrl_ptr = (int *)PIXEL_BUF_CTRL_BASE;
    /* Read location of the pixel buffer from the pixel buffer controller */
    pixel_buffer_start = *pixel_ctrl_ptr;
	
	//game over screen text
    clear_screen();
	char over[] = "Game Over";
	drawString(over, 60, 20, 15, CYAN);
	
	drawString("REPLAY", 70, 80, 10, GREEN);
	fillRectangleNew(185, 85, 40, 17, GREEN);
	drawString("ENTER", 190, 90, 3, BLACK);
	
	drawString("MENU", 100, 130, 10, GREY);
	fillRectangleNew(185, 135, 40, 17, GREY);
	drawString("ESC", 190, 140, 3, BLACK);
	
	//Print score
	drawString("SCORE", 90, 180, 5, GREY);
	char scoreAry[20];
	snprintf(scoreAry, 20, "%d", score);
	drawString(scoreAry, 200, 180, 5, GREY);
	
	//Print highscore
	drawString("HIGHSCORE", 90, 200, 5, GREY);
	char highscoreAry[20];
	snprintf(highscoreAry, 20, "%d", highScore);
	drawString(highscoreAry, 200, 200, 5, GREY);
	
	
	volatile int * PS2_ptr = (int *)PS2_BASE;
	int PS2_data, RVALID;
	char byte1 = 0, byte2 = 0, byte3 = 0;
	
	*(PS2_ptr) = 0xFF; // reset
	
	//checking for input
	while (1) {
		PS2_data = *(PS2_ptr); // read the Data register in the PS/2 port
		RVALID = PS2_data & 0x8000; // extract the RVALID field
		if (RVALID) {
			/* shift the next data byte into the display */
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
			HEX_PS2(byte1, byte2, byte3);
			
			if ((byte1 == (char)0x76) && (byte2 == (char)0xF0) && (byte3 == (char)0x76)) {	
				printf("ESC clicked. Back to Menu\n");	
				menuOn = true;
				gameOver = false;
				helpOn = false;
				resetVariables();
				return;
			}else if ((byte1 == (char)0x5A) && (byte2 == (char)0xF0) && (byte3 == (char)0x5A)) {	
				printf("Enter clicked. Proceed to Game State\n");
				menuOn = false;
				gameOver = false;
				gameOn = true;
				resetVariables();
				return;
			}
		}
	}
}

void resetVariables(){
	/* Ninja Movement */
	dx = 0;
	shifting = false;
	xloc = 30;
	yloc = 190;
	xSz = 19;
	ySz = 19;

	/* Obstacle Movement */
	xObs = 30;
	yObs = 0;
	xObsSz = 80;
	yObsSz = 10;
	obsDy = 7;
	generated = false;
	
	/* Item movement */
	xItem = 154;
	yItem = 30;
	xItemSz = 10;
	yItemSz = 10;
	itemDy = 10;
	itemDrop = false;
	itemEffect = false;
	itemHit = false;

	/* Generated side and misc initial variables */ 
	side = 0;
	type = 0;
	width = 0;
	firstTime = true;
	obsHit = false;
	loopCounter = 0;
	score = 0;
}

void clear_screen() {
    for (int i = 0; i< RESOLUTION_X; i++) {
        for (int j = 0; j < RESOLUTION_Y; j++) {
            *(short int *)(pixel_buffer_start + (j << 10) + (i << 1)) = 0;
        }
    }
}

void clear_screen_except_border(int xMin, int yMin, int xMax, int yMax) {
    for (int i = xMin; i< xMax; i++) {
        for (int j = yMin; j < yMax; j++) {
            *(short int *)(pixel_buffer_start + (j << 10) + (i << 1)) = 0;
        }
    }
}

void clearFIFO() {
    volatile int * PS2_ptr = (int *)PS2_BASE;
    int PS2_data, RAVAIL, RVALID;
    // PS/2 mouse needs to be reset (must be already plugged in)
    *(PS2_ptr) = 0xFF; // reset
    PS2_data = *(PS2_ptr);
    RVALID = PS2_data & 0x8000;
    //if (RVALID != 1) { printf("Rvalid not 1\n"); return;}
    RAVAIL = PS2_data & 0xFFFE0000;
    for (int i = 0; i < RAVAIL -1 ; i++) {
        PS2_data = *(PS2_ptr); //printf("yes\n");
    }
}

void wait_for_vsync() {
    volatile int * pixel_ctrl_ptr = (int *)0xFF203020;
    register int status;
    
    *pixel_ctrl_ptr = 1; //start synchronization process
    
    status = *(pixel_ctrl_ptr + 3);
    while ((status & 0x01) != 0) {
        status = * (pixel_ctrl_ptr + 3);
    }
}

void drawA7Segment(int segCode[], int xmin, int ymin, int segLength, short int color) {
	if (segCode[0]) draw_line(xmin, ymin, xmin + segLength, ymin, color);
	if (segCode[1]) draw_line(xmin + segLength, ymin, xmin + segLength, ymin + segLength, color);
	if (segCode[2]) draw_line(xmin + segLength, ymin + segLength, xmin + segLength, ymin + 2*segLength, color);
	if (segCode[3]) draw_line(xmin + segLength, ymin + 2*segLength, xmin, ymin + 2*segLength, color);
	if (segCode[4]) draw_line(xmin, ymin + 2*segLength, xmin, ymin + segLength, color);
	if (segCode[5]) draw_line(xmin, ymin + segLength, xmin, ymin, color);
	if (segCode[6]) draw_line(xmin, ymin + segLength, xmin + segLength, ymin + segLength, color);
}

void decToBinary7Bits(int n, int *correctOrder)
{
    // array to store binary number
    int binaryOpp[7];
 
    // counter for binary array
    int i = 0;
    while (n > 0) {
 
        // storing remainder in binary array
        binaryOpp[i] = n % 2;
        n = n / 2;
        i++;
    }
    
    int counter = 0;
    for (; counter < 7 - i; counter ++) {
        correctOrder[counter] = 0;
    }
    
    for (int j = i - 1; j >= 0; j--, counter++) {
        correctOrder[counter] = binaryOpp[j];
        //printf("%d\n", correctOrder[counter]);
    }
    
}

/* Invalid letters are mapped to all segments off (0x00). */
int decode_7seg(unsigned char chr)
{ /* Implementation uses ASCII */
    if (chr > (unsigned char)'z')
        return 0x00;
    return seven_seg_digits_decode[chr - '0'];
}

void fillRectangle(int x, int y, int xLength, int yLength, short int color) {
    for (int i = x; i < xLength+x; i++) {
        for (int j = y; j < yLength+y; j++) {
            plot_pixel(i, j, color);
        }
    }
}

void fillRectangleNew(int x, int y, int xLength, int yLength, short int color) {
	for (int i = x; i < xLength+x; i++) {
		for (int j = y; j < yLength+y; j++) {
			plot_pixel(i, j, color);
		}
	}
}

void swap(int * a, int * b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void drawString(char *str, int xmin, int ymin, int segLength, short int color) {
    int spacing;
    if (segLength >= 6){
		spacing = 8;
	}
    else if (segLength < 6){
		spacing = 4;
	}
    for (int i = 0; str[i] != '\0'; i++, xmin += segLength + spacing) {
        if (str[i] != ' ') {
            int codeInt = decode_7seg(str[i]);
            int codeAry[7];
            decToBinary7Bits(codeInt, codeAry);
            drawA7Segment(codeAry, xmin, ymin, segLength, color);
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, short int color) {
	bool is_steep = ABS(y1 - y0) > ABS(x1 - x0);
	if (is_steep) {
		swap(&x0, &y0);
		swap(&x1, &y1);
		
	}
	if (x0 > x1) {
		swap(&x0, &x1);
		swap(&y0, &y1);
		
	}		
	int deltax = x1 - x0;
	int deltay = ABS(y1 - y0);
	int error = -(deltax / 2);
	int y = y0;
	int y_step;
	if (y0 < y1) y_step = 1;
	else y_step = -1;
	
	for (int x = x0; x <= x1; x++) {
		if (is_steep) 
			plot_pixel(y, x, color);
			
		else
			plot_pixel(x, y, color);
		error = error + deltay;
		if (error >= 0) {
			y = y + y_step;
			error = error - deltax;
		}
	}
}	

void plot_pixel(int x, int y, short int line_color)
{
    *(short int *)(pixel_buffer_start + (y << 10) + (x << 1)) = line_color;
}


