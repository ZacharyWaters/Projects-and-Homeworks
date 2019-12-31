/**
* @file graphics.c
* @author YOUR NAME HERE
* @date DATE HERE
* @brief A graphics library for drawing geometry, for Homework 8 of Georgia Tech
*        CS 2110, Spring 2018.
*/

// Please take a look at the header file below to understand what's required for
// each of the functions.
#include "graphics.h"
#include<stddef.h>

Pixel noFilter(Pixel c) {
    // This is the correct implementation, please do not edit it.
    return c;
}

// TODO: Complete according to the prototype in graphics.h
Pixel greyscaleFilter(Pixel c) {  
	//getting the color channels
	Pixel red = c & 31;
	Pixel green = c & 992;
	Pixel blue = c & 31744;
	//shifting all channels into a single area
	red = red;
	green = green >> 5;
	blue = blue >> 10;	
	//applyinng the weights for human perception
	red = red * 77;
	green = green * 151;
	blue = blue * 28;
	//adding all the channels together
	Pixel x = red+green+blue;
	//right shifting
	x = x >> 8;
	//copying x to other channels
	Pixel y = x << 5;
	Pixel z = x << 10;
	Pixel result = x + y + z; 
	return result;
}

// TODO: Complete according to the prototype in graphics.h
Pixel redOnlyFilter(Pixel c) {
	Pixel x = c;   
	x = x & 31;
	return x;
}

// TODO: Complete according to the prototype in graphics.h
Pixel brighterFilter(Pixel c) {
	//getting the color channels
	Pixel red = c & 31;
	Pixel green = c & 992;
	Pixel blue = c & 31744;
	//max - channel
	Pixel x = 31 - red;
	Pixel y = 992 - green;
	Pixel z = 31744 - blue;
	//shifting diffs to one side
	y = y >> 5;
	z = z >> 10;
	//shifting channels to one side
	green = green >> 5;
	blue = blue >> 10;
	//right shifting diff
	x = x >> 1;
	y = y >> 1;
	z = z >> 1;
	//getting new channel
	red = red + x;
	green = green + y;
	blue = blue + z;
	//moving channels back to original positions
	green = green << 5;
	blue = blue << 10;
	//combining channels
	Pixel result = red + green + blue;
    	return result;
}

// TODO: Take a look at this function, which we have readily implemented,
// to see the coding style and logic. This function is not required, delete it
// if you want, or keep it if it's useful to look at for help.
void drawHorizontalLine(Screen *screen, Line *line) {
    // Check if the line is actually horizontal
    if (line->start.y != line->end.y) {
        return;
    }

    // Okay, so it is horizontal. Let's find which one is the left-side vertex
    unsigned int l = line->start.x <= line->end.x ? line->start.x : line->end.x;
    unsigned int r = line->start.x > line->end.x ? line->start.x : line->end.x;

    // Keep a vector that we can pass to drawPixel
    Vector v = {0, line->start.y};
    for (unsigned int x = l; x <= r; x++) {
        v.x = x;
        drawPixel(screen, v, line->color);
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawPixel(Screen *screen, Vector coordinates, Pixel pixel) {
	unsigned int screenx = screen -> size.x;
	unsigned int screeny = screen -> size.y;
	unsigned int x = coordinates.x;
	unsigned int y = coordinates.y;
	if (!((x >= screenx) || (y >= screeny))) {
        screen->buffer[((y * screenx) + x)] = pixel;
    }
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledRectangle(Screen *screen, Rectangle *rectangle) {
	Pixel color = rectangle -> color;	
	Vector coordinates = rectangle -> top_left;
	int width = rectangle -> size.x;
	int height = rectangle -> size.y;
	int ybackup = coordinates.y;
	for(int i=0; i<width;i++){
		for(int j=0; j<height;j++){
			drawPixel(screen, coordinates,color);
			coordinates.y = coordinates.y + 1;
					}
			coordinates.x = coordinates.x + 1;
			coordinates.y = ybackup; 
				}	
}

// TODO: Complete according to the prototype in graphics.h
void drawLine(Screen *screen, Line *line) {
    UNUSED(screen);
    UNUSED(line);
	Pixel color = line -> color;	
	Vector startcoordinates = line -> start;
	Vector endcoordinates = line -> end;
	int dx = endcoordinates.x - startcoordinates.x;
	int dy = endcoordinates.y - startcoordinates.y;
	int signx = 1;
	int signy = 1;
	int temp = 0;
	int changed = 0;
	if(dx < 0 ){
		dx = dx * -1;
		signx = -1;
	}
	if(dx == 0 ){
		signx = 0;
	}
	if(dy < 0 ){
		dy = dy * -1;
		signy = -1;
	}
	if(dy == 0 ){
		signy = 0;
	}	
        if (dy > dx){
            	temp=dy;
		dy = dx;
		dx = temp;
              	changed = 1;
        }
	int e = (2 * dy) - dx;
	int twodx = 2 * dx;
	int twody = 2 * dy;

	for(int i = 1; i <= dx; i++ ) {//A
		drawPixel(screen, startcoordinates,color);
		while (e >= 0) {//B
				if (changed==1){
					startcoordinates.x = startcoordinates.x + signx;}
				else{
					startcoordinates.y = startcoordinates.y + signy;}
				e = e - twodx;		
				}//B
		if (changed==1){
			startcoordinates.y = startcoordinates.y + signy;}
		 else{
			startcoordinates.x = startcoordinates.x + signx;}
			e = e + twody;
		}//A
	drawPixel(screen,endcoordinates,color);
}

// TODO: Complete according to the prototype in graphics.h
void drawPolygon(Screen *screen, Polygon *polygon) {
	
	Pixel color = polygon -> color;	
	int amount =  polygon -> num_vertices;
	Vector* vertices = polygon -> vertices;
	for(int i=0;i<(amount-1);i++){
		Line l = {vertices[i],vertices[i+1],color};
		drawLine(screen, &l);
	}
		Line e = {vertices[amount-1],vertices[0],color};
		drawLine(screen, &e);
}

// TODO: Complete according to the prototype in graphics.h
void drawRectangle(Screen *screen, Rectangle *rectangle) {
	Pixel color = rectangle -> color;	
	unsigned int width = rectangle -> size.x;
	unsigned int height = rectangle -> size.y;	
	Vector a = rectangle -> top_left;
	Vector b = {a.x+width-1, a.y};
	Vector c = {a.x+width-1, a.y+height-1};
	Vector d = {a.x, a.y+height-1};	
	Vector shape[4];
	shape[0]=a;
	shape[1]=b;
	shape[2]=c;
	shape[3]=d;
	Polygon poly = {shape,4,color};
	drawPolygon(screen, &poly);
}


// TODO: Complete according to the prototype in graphics.h
void drawCircle(Screen *screen, Circle *circle) {	
	int mx =  circle -> center.x;
	int my =  circle -> center.y;
	int x = 0;
	int y = circle -> radius;
	int d = 1 - y;
	while(x<=y){ // A
			Vector v1 = {mx+x,my+y};
			drawPixel(screen,v1,circle -> color);
		
		        Vector v2 = {mx+x,my-y};
			drawPixel(screen,v2,circle -> color);
	
			Vector v3 = {mx-x,my+y};
			drawPixel(screen,v3,circle -> color);

			Vector v4 = {mx-x,my-y};
			drawPixel(screen,v4,circle -> color);
		
			Vector v5 = {mx+y,my+x};
			drawPixel(screen,v5,circle -> color);
		
		        Vector v6 = {mx+y,my-x};
			drawPixel(screen,v6,circle -> color);
	
			Vector v7 = {mx-y,my+x};
			drawPixel(screen,v7,circle -> color);

			Vector v8 = {mx-y,my-x};
			drawPixel(screen,v8,circle -> color);

			if(d<0) { //B
  		                    d = d + 2 * x + 3;
  		                    x += 1;
				} //B
			else{ //C
				 d = d + 2 * (x-y) + 5;
		                 x += 1;
				 y -= 1;
			    } //C
		   } //A
}

// TODO: Complete according to the prototype in graphics.h
void drawFilledCircle(Screen *screen, Circle *circle) {
	Pixel color = circle -> color;
	Vector center = circle -> center;
	int mx = center.x;
	int my = center.y;
	unsigned int radius = circle -> radius;	
	int x = 0;
	int y = radius;
	int d = 1 - radius;
	Vector p1;
	Vector p2;
	Vector p3;
	Vector p4;
	Vector p5;
	Vector p6;
	Vector p7;
	Vector p8;
	while(x <= y){ //A
			p1.x = mx + x;
			p1.y = my + y;
			if(my > y){ //B
			p2.x = mx + x;
			p2.y = my - y;
				   }//B
			else{       //C
			p2.x = mx + x;
			p2.y = 0; 
				   }//C
			Line line1 = {p1, p2, color};			
			drawLine(screen, &line1);

			if(mx >= x) {//D
			p3.x = mx - x;
			p3.y = my + y;

			if(my > y) {//E
			p4.x = mx - x;
			p4.y = my - y;
				   }//E
			else{       //F
			p4.x = mx - x;
			p4.y = 0;
			           }//F
			Line line2 = {p3, p4, color};			
			drawLine(screen, &line2);
				    }//D
			
			p5.x = mx + y;
			p5.y = my + x;
			if(my > x){//G
			p6.x = mx + y;
			p6.y = my - x;
				   }//G
			else{       //H
			p6.x = mx + y;
			p6.y = 0; 
				   }//H
			Line line3 = {p5, p6, color};			
			drawLine(screen, &line3);

			if (mx >= y) {//I
			p7.x = mx - y;
			p7.y = my + x;
			if (my > x) {//J
			p8.x = mx - y;
			p8.y = my - x;
				    }//J
			else{        //K
			p8.x = mx - y;
			p8.y = 0;
			    }	     //K
			Line line4 = {p7, p8, color};			
			drawLine(screen, &line4);
				     }//I
	                 if (d < 0) { //L
	                 d = d + 2 * x + 3;
	                 x = x + 1;
				    }//L
			else{         //M
		       d = d + 2 * (x-y) + 5;
                       x = x + 1;
                       y = y - 1;
			    }         //M
		     } //A
}

// TODO: Complete according to the prototype in graphics.h
void drawImage(Screen *screen, Image *image, CropWindow *cropWindow,
                 Pixel (*colorFilter)(Pixel)) {
    UNUSED(cropWindow);
    UNUSED(colorFilter);
	//Pixel colors = image ->buffer[0];
	Vector coordinates = image -> top_left;
	int width = image -> size.x;
	int height = image -> size.y;
	int xbackup = coordinates.x;
	int counter = 0;
	int cropison = 0;
	int cropcounter = 0;
	int cropwidth = 0;
	int cropheight = 0;
	int cropwidthcopy = 0;
	Vector cropstart=coordinates;
	if(!(cropWindow == NULL)){
	cropison=1;
	cropstart = cropWindow -> top_left;
	cropwidth = cropWindow -> size.x;
	cropheight = cropWindow -> size.y;
	cropwidthcopy = cropwidth;
	}
	for(int i=0; i<height;i++){
		for(int j=0; j<width;j++){
			Pixel colors = image ->buffer[counter];
			Pixel filter = colorFilter(colors);
			if(cropison==0){//A
			drawPixel(screen, coordinates,filter);
			}//A
			if(((cropison==1)&&(cropheight!=0)&&(cropwidth!=0)&&(cropcounter==1))||
((coordinates.x==cropstart.x)&&(coordinates.y==cropstart.y)&&(cropison==1))
){//B
			drawPixel(screen, coordinates,filter);
			cropcounter=1;
			cropwidth = cropwidth -1;
			}//B
			coordinates.x = coordinates.x + 1;
			counter = counter +1;
					}
			coordinates.y = coordinates.y + 1;
			coordinates.x = xbackup; 
			cropheight = cropheight -1;
			cropwidth = cropwidthcopy;
				}
}
