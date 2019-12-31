#include "myLib.h"
#include "Methods.h"
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r)) 

//unsigned short *videoBuffer = (unsigned short *)0x6000000;
volatile unsigned short *videoBuffer = (volatile unsigned short *)0x6000000;


// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

//DRAW RECTANGLE METHOD
void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
		for(int c = 0; c<width; c++) {
			setPixel(row+r, col+c, color);
		}
	}
}

//DELAY METHOD
void delay(int n) {
	volatile int x = 0;
	for(int i=0; i<n*8000; i++) {
		x++;
	}
}

//WAIT FOR BLANK METHOD
void waitForVblank() {
	while(SCANLINECOUNTER > 160)
		;
	while(SCANLINECOUNTER<160)
		;
}

//GET PIXEL COLOR METHOD
unsigned short getPixelColor(int row, int col){
	return videoBuffer[OFFSET(row,col,240)];
}

//IS GROUND METHOD
int isGround(int row, int col){
//GROUND1 COLOR = 0x1950
//GROUND2 COLOR = 0x2dfc
//GROUND3 COLOR = 0x157b
//GROUND4 COLOR = 0x6318
//GROUND5 COLOR = 0x2d6e
unsigned short below;
for(int i=0;i<11;i++){
	below = getPixelColor(row+13, col+i);
	if((below == 0x1950)||(below==0x2dfc)||(below==0x157b)
	   ||(below==0x6318)||(below==0x2d6e)  ){
		return 1;
			  }
		      }
		return 0;
}

//IS METEORTOUCH METHOD
int meteortouch(int row, int col){
unsigned short below;
for(int i=0;i<13;i++){
	below = getPixelColor(row+19, col+i);
	if((below == 0x1950)||(below==0x2dfc)||(below==0x157b)
	   ||(below==0x6318)||(below==0x6318)||(below==0x2d6e)
           ||(below==0x7fff)||(below==0x56b5)||(below==0x294a)   ){
		return 1;
			  }
		      }
		return 0;
}


//IS LOSE METHOD
int lose(int row, int col){
//GROUND1 COLOR = 0x1950
//GROUND2 COLOR = 0x2dfc
//GROUND3 COLOR = 0x157b
//GROUND4 COLOR = 0x6318
//GROUND5 COLOR = 0x2d6e
//CHARACTER GREENBLOCK COLOR = 0x26c4
//CHARACTER ASTRO COLOR1 = 0x0000
//CHARACTER ASTRO COLOR2 = 0x7fff
//CHARACTER ASTRO COLOR3 = 0x56b5
if(row>=160){
	return 2;
	    }
unsigned short check;
for(int i=0;i<14;i++){
	check = getPixelColor(row+i, col);
	if(       (check != 0x1950)&&(check != 0x2dfc)&&(check != 0x157b)
		&&(check != 0x6318)&&(check != 0x2d6e)&&(check != 0x26c4)
                &&(check != BLACK) &&(check != 0x7fff)&&(check != 0x56b5)  ){
		return 1;
		          }
	check = getPixelColor(row+i, col+9);
	if(       (check != 0x1950)&&(check != 0x2dfc)&&(check != 0x157b)
		&&(check != 0x6318)&&(check != 0x2d6e)&&(check != 0x26c4)
                &&(check != BLACK) &&(check != 0x7fff)&&(check != 0x56b5)  ){
		return 1;
		          }
		     }
for(int i=0;i<10;i++){
	check = getPixelColor(row-1, col+i);
	if(       (check != 0x1950)&&(check != 0x2dfc)&&(check != 0x157b)
		&&(check != 0x6318)&&(check != 0x2d6e)&&(check != 0x26c4)
                &&(check != BLACK) &&(check != 0x7fff)&&(check != 0x56b5)  ){
		return 1;
		          }
	check = getPixelColor(row+12, col+i);
	if(       (check != 0x1950)&&(check != 0x2dfc)&&(check != 0x157b)
		&&(check != 0x6318)&&(check != 0x2d6e)&&(check != 0x26c4)
                &&(check != BLACK) &&(check != 0x7fff)&&(check != 0x56b5)  ){
		return 1;
		          }
		      }
return 0;
}


//CAN MOVE RIGHT METHOD
int canMoveRight(int row, int col){
unsigned short rightside;
for(int i=0;i<13;i++){
	rightside = getPixelColor(row+i, col+11);
	if((rightside == 0x1950)||(rightside==0x2dfc)||(rightside==0x157b)
	   ||(rightside==0x6318)||(rightside==0x6318)  ){
		return 0;
			  }
		      }
		return 1;
}


//CAN MOVE LEFT METHOD
int canMoveLeft(int row, int col){
unsigned short leftside;
for(int i=0;i<13;i++){
	leftside = getPixelColor(row+i, col-1);
	if((leftside == 0x1950)||(leftside==0x2dfc)||(leftside==0x157b)
	   ||(leftside==0x6318)||(leftside==0x6318)  ){
		return 0;
			  }
		      }
		return 1;
}

//CAN MOVE UP METHOD
int canMoveUp(int row, int col){
unsigned short upside;
for(int i=0;i<10;i++){
	upside = getPixelColor(row-1, col+i);
	if((upside == 0x1950)||(upside==0x2dfc)||(upside==0x157b)
	   ||(upside==0x6318)||(upside==0x6318)  ){
		return 0;
			  }
		      }
		return 1;
}

//DrAW IMAGE METHOD
void drawImage3 (int r, int c, int width, int height, const unsigned short* image){
	for(int i = 0; i<height;i++){
	DMA[DMA_CHANNEL_3].src = image + OFFSET(i,0,width);
	DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(r+i,c,240);	
	DMA[DMA_CHANNEL_3].cnt = DMA_ON | width;	
	}
}
