typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char u8;

#define UNUSED_PARAM(param) ((void)((param)))

#define REG_DISPCTL *(u16 *)0x4000000
#define MODE3 3
#define BG2_ENABLE (1<<10)

#define COLOR(r,g,b) ((r) | (g) << 5 | (b) << 10)
#define RED COLOR(31,0,0)
#define WHITE COLOR(31,31,31)
#define GREEN COLOR(0, 31, 0)
#define BLACK 0

#define OFFSET(r,c) ((r)*240+(c))


unsigned short *videoBuffer = (u16 *)0x6000000;

void setPixel(int row, int col, u16 color);
void drawRectangle(int row, int col, int width, int height, u16 color);
void drawHollowRectangle(int row, int col, int width, int height, u16 color);

int main()
{
	REG_DISPCTL = MODE3 | BG2_ENABLE;

	// use the functions you wrote to draw a pretty picture here!

	setPixel(10, 10, RED);
	drawRectangle(20, 20, 20, 20, WHITE);
	drawHollowRectangle(40, 40, 20, 20, GREEN);

	while(1);
}

void setPixel(int row, int col, u16 color)
{
    // TODO: create a function that will set a pixel at the specified
    //       pixel to the color passed in
	videoBuffer[OFFSET(row,col)]=color;
}

void drawRectangle(int row, int col, int width, int height, u16 color)
 {	
for(int i =0; i < height; i++){
	for(int j = 0; j<width; j++){
		setPixel(i+row,j+col,color);
}
}
}
void drawHollowRectangle(int row, int col, int width, int height, u16 color)
{
    // TODO: create a function that will draw the outline of a rectangle
    //       of the specified color. row and col should refer to the top
    //       left of the rectangle.
    //
    // this function needs to be efficient and should not run in O(n*n)
    // time!
	 for (int i = 0; i < width; i++) {
		setPixel(i+row,col,color);
		setPixel(i+row,col+width,color);
	} 
	 for (int j = 0; j <= height; j++) { 
		setPixel(row,j+col,color);
		setPixel(row+height,j+col,color);}
}
