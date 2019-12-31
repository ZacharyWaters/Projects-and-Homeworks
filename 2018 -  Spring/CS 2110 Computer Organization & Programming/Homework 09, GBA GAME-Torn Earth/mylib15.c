#include "mylib.h"
#include "DMA.h"

// Prototypes
void setPixel(int , int , unsigned short );
void drawRect(int row, int col, int height, int width, unsigned short color);
void delay(int n);
void waitForVblank();
int rand();

unsigned short *videoBuffer = (unsigned short *)0x6000000;


// setPixel -- set the pixel at (row, col) to color
void setPixel(int row, int col, unsigned short color) {
	videoBuffer[OFFSET(row, col, 240)] = color;
}

void drawRect(int row, int col, int height, int width, unsigned short color) {
	for(int r=0; r<height; r++) {
		/*
		for(int c = 0; c<width; c++) {
			setPixel(row+r, col+c, color);
		}
		*/
		DMA[3].src = &color;
		DMA[3].dst = &videoBuffer[OFFSET(row + r, col, 240)];
		DMA[3].cnt = width | DMA_ON | DMA_SOURCE_FIXED;
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

