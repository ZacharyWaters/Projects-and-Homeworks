#include "myLib.h"
#define OFFSET(r, c, rowlen) ((c) + (rowlen)*(r)) 

u16* videoBuffer = (u16*)0x6000000;

void setPixel(int row, int col, u16 color) {
    videoBuffer[row * 240 + col] = color;
}

void waitForVblank()
{
    while(SCANLINECOUNTER > 160);
    while(SCANLINECOUNTER < 160);
}

void drawRedRectangle(int row, int col, int width, int height) {
   	unsigned short redcolor = RED;
	for(int i=0;i<height;i++){
	DMA[3].src = &redcolor;
	DMA[3].dst = &videoBuffer[OFFSET(row+i,col,240)];
	DMA[3].cnt = (width)|DMA_SOURCE_FIXED|DMA_ON;
				}
	/** 
   for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            setPixel(row + r, c + col, RED);
        }
    }
   	*/
}

void drawImage(int row, int col, int width, int height, const u16* image) {
for(int i = 0; i<height;i++){
	DMA[DMA_CHANNEL_3].src = image + OFFSET(i,0,width);
	DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row+i,col,240);	
	DMA[DMA_CHANNEL_3].cnt = DMA_ON | width;	
			    }
/**
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            u16 image_pixel = image[r * width + c];
            setPixel(row + r, c + col, image_pixel);
        }
    }
*/
}

void drawImageFlippedLR(int row, int col, int width, int height, const u16* image) {
///**
for(int i = 0; i<height;i++){
	DMA[DMA_CHANNEL_3].src = image + OFFSET(i,0,width);
	DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row+i,col+119,240);	
	DMA[DMA_CHANNEL_3].cnt = DMA_ON|width|DMA_DESTINATION_DECREMENT;	
			    }
//*/
/**  
    for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            u16 image_pixel = image[r * width + width - c - 1];
            setPixel(row + r, c + col, image_pixel);
        }
    }
*/
}

void drawImageFlippedUD(int row, int col, int width, int height, const u16* image) {
for(int i = 0;i<height;i++){
	DMA[DMA_CHANNEL_3].src = image + OFFSET(i,0,width);
	DMA[DMA_CHANNEL_3].dst = videoBuffer + OFFSET(row+height-1-i,col,240);	
	DMA[DMA_CHANNEL_3].cnt = DMA_ON|width;
			    }

/**
     for (int r = 0; r < height; r++) {
        for (int c = 0; c < width; c++) {
            u16 image_pixel = image[(height - r - 1) * width + c];
            setPixel(row + r, c + col, image_pixel);
        }
    }
*/
}
