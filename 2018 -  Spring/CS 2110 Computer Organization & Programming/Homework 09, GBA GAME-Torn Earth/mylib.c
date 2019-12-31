#include "mylib.h"

// Prototypes
void setPixel(int , int , unsigned short );
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();
int rand();
int canMoveRight(int row, int col);
int isGround(int row, int col);
int canMoveLeft(int row, int col);
unsigned short getPixelColor(int row, int col);

unsigned short *videoBuffer = (unsigned short *)0x6000000;


// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
		for(int c = 0; c<width; c++) {
			setPixel(row+r, col+c, color);
		}
	}
}

void delay(int n) {
	volatile int x = 0;
	for(int i=0; i<n*8000; i++) {
		x++;
	}
}

void waitForVblank() {
	while(SCANLINECOUNTER > 160)
		;
	while(SCANLINECOUNTER<160)
		;
}

unsigned short getPixelColor(int row, int col){
	return videoBuffer[OFFSET(row,col,240)];
}


int isGround(int row, int col){
unsigned short below1 = getPixelColor(row+11, col);
unsigned short below2 = getPixelColor(row+11, col+5);
unsigned short below3 = getPixelColor(row+11, col+10);
if((below1 == WHITE)||(below2==WHITE)||(below3==WHITE)) {
	return 1;  }
else {
	return 0;  }
}

int canMoveRight(int row, int col){
unsigned short below1 = getPixelColor(row, col+11);
unsigned short below2 = getPixelColor(row+5, col+11);
unsigned short below3 = getPixelColor(row+9, col+11);
if((below1 == WHITE)||(below2==WHITE)||(below3==WHITE)) {
	return 0;  }
else {
	return 1;  }
}

int canMoveLeft(int row, int col){
unsigned short below1 = getPixelColor(row, col-1);
unsigned short below2 = getPixelColor(row+5, col-1);
unsigned short below3 = getPixelColor(row+9, col-1);
if((below1 == WHITE)||(below2==WHITE)||(below3==WHITE)) {
	return 0;  }
else {
	return 1;  }
}

void drawImage3 (int r, int c, int width, int height, const unsigned short* image){
	for(int i = 0; i<height;i++){
	DMA[DMA_CHANNEL_3].src = image + OFFSET(i,0,width);
	DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(r+i,c,240);	
	DMA[DMA_CHANNEL_3].cnt = DMA_ON | width;	
	}
}

