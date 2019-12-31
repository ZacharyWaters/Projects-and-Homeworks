/**
* @file graphics.h
* @author Cem Gokmen
* @date March 5, 2018
* @brief A graphics library for drawing geometry, for Homework 8 of Georgia Tech
* CS 2110, Spring 2018.
*/

#include "geometry.h"
#define UNUSED(x) (void)(x)

/**
* @brief Return the provided pixel without any changes (identity filter).
* @param c The Pixel that you should return directly
* @return The pixel received as the input.
*/
Pixel noFilter(Pixel c);

/**
* @brief Return a greyscale version of the provided pixel.
* @param c The Pixel to convert to greyscale.
* @return The greyscale version of the provided pixel.
* @details You need to implement this function such that the greyscale pixel's
*          all three channels are set to the value n such that:
*              n = ((r*77)+(g*151)+(b*28)) >> 8
*          where r, g and b are the red, green and blue channels of the original
*          pixel.
*
*          The variable coefficient for the pixels account for the human eye's
*          more weighted perception of green color.
*/
Pixel greyscaleFilter(Pixel c);

/**
* @brief Return a red-only version of the provided pixel.
* @param c The Pixel to apply the red-only filter to.
* @return The red-only version of the provided pixel.
* @details The operation expected here is that you mask the given pixel so that
*          the green and blue channels are zeroed out while the value of the red
*          channel is exactly maintained.
*/
Pixel redOnlyFilter(Pixel c);

/**
* @brief Return a lighter version of the provided pixel.
* @param c The Pixel to apply the brightness filter to.
* @return The brighter version of the provided pixel.
* @details The operation expected here is that you apply 50% white on the color.
*          To achieve this, repeat the following procedure for each of the color
*          channels:
*              current = the original value of the channel
*              max = the maximum value this channel can hold (what is it?)
*              diff = max - current
*              new = current + (diff >> 1)
*              save `new` as the new value for the channel
*/
Pixel brighterFilter(Pixel c);

/**
* @brief Draw a single pixel.
* @param screen The Screen to draw onto.
* @param coordinates The coordinates to draw the pixel onto.
* @param pixel The Pixel / color to draw.
* @details Your implementation needs to use the screen's width and height to
*          find the correct position to draw onto and then write onto that index
*          in the screen's buffer. You need to return without drawing if the
*          call is attempting to draw outside the bounds of the screen, (x < 0,
*          x >= width, y < 0, y >= height).
*
*          To calculate the position of the pixel in the 1-d pixel array, take a
*          look at the diagram provided in the assignment documentation in
*          Section 2.2
*/
void drawPixel(Screen *screen, Vector coordinates, Pixel pixel);

/**
* @brief Draw the filled rectangle provided, onto the screen provided.
* @param screen The Screen to draw onto.
* @param rectangle The Rectangle to draw.
* @details To draw this rectangle, you should iterate over each row and each
*          column inside the rectangle (including bits border) and use the
*          drawPixel function to draw the rectangle's color onto the buffer.
*
*          Even though this is the simplest of the shapes you will draw, here's
*          pseudocode to get you started:
*
*          drawRectangle(x, y, width, height, color):
*              for every r from 0 (inclusive) to height (exclusive)
*                  for every c from 0 (inclusive) to width (exclusive)
*                      drawPixel(x + c, y + r, color)
*/
void drawFilledRectangle(Screen *screen, Rectangle *rectangle);

/**
* @brief Draw the line provided, onto the screen provided.
* @param screen The Screen to draw onto.
* @param line The Line to draw.
* @details To draw a line, you will need to use an algorithm named Bresenham's
*          Line Drawing algorithm, which is explained below in pseudocode:
*
*          bresenhamLine(int x1, int y1, int x2, int y2) {
*              changed = false
*              int x = x1;
*              int y = y1;
*              int dx = |x2 – x1|;
*              int dy = |y2 – y1|;
*              int signx = signum(x2 – x1);
*              int signy = signum(y2 – y1);
*
*              if (dy > dx){
*                  swap(dx, dy);
*                  changed = true;
*              }
*
*              int e = 2 * dy - dx;
*              for (int i = 1; i <= dx; i++) {
*                  plot(x, y);
*                  while (e >= 0) {
*                      if (changed)
*                          x = x + signx;
*                      else
*                          y = y + signy;
*                      e = e – 2 * dx;
*                  }
*                  if (changed)
*                      y += signy;
*                  else
*                      x += signx;
*                  e = e + 2 * dy;
*              }
*
*              plot(x2, y2) // IMPORTANT! WE JUST ADDED THIS LINE!
*          }
*
*          Important! Your code CANNOT import any additional library, so you
*          can implement the absolute values using ternaries and NOT stdlib!
*
*          Important! The pseudocode talks about a "signum" function. Signum is
*          Latin for sign, and the function is simply used to get the sign of an
*          integer. Therefore signum(x) is equal to:
*               -1 if x < 0,
*               0 if x = 0,
*               1 if x > 0.
*
*          You therefore also need to use a ternary for this since such a func.
*          is not avaiable for you to use without using the standard libraries.
*
*          Please try to read the document and understand how the Bresenham line
*          algorithm works - however, it is sufficient for you to implement the
*          Java-pseudocode provided in the document.
*/
void drawLine(Screen *screen, Line *line);

/**
* @brief Draw the polygon provided, onto the screen provided.
* @param screen The Screen to draw onto.
* @param polygon The Polygon to draw.
* @details The drawPolygon function will draw a polygon consisting of lines that
*          connect an arbitrary number of points (vertices). The drawn polygon
*          will not need to be filled. Your implementation should simply loop
*          through the vertices of the polygon struct in the order they appear
*          in the array, drawing lines between the adjacent vertices, as well as
*          between the last vertex and the first.
*
*          You can assume that the polygons will have at least two vertices.
*/
void drawPolygon(Screen *screen, Polygon *polygon);

/**
* @brief Draw the rectangle provided, onto the screen provided.
* @param screen The Screen to draw onto.
* @param rectangle The Rectangle to draw.
* @details We will interpret this (unfilled) rectangle as a Polygon with 4
*          vertices. As a result, your implementation should use the Rectangle
*          provided to create a Polygon object and then draw this Polygon using
*          the drawPolygon function. No actual drawing should be done in this
*          function.
*/
void drawRectangle(Screen *screen, Rectangle *rectangle);

/**
* @brief Draw the (unfilled) circle provided, onto the screen provided.
* @param screen The Screen to draw onto.
* @param circle The Circle to draw.
* @details To draw a circle, you will need to use an algorithm named Bresenham's
*          Circle Drawing algorithm, which is explained below in pseudocode:
*
*          bresenhamCircle(int mx, int my, int radius) {
*              int x = 0;
*              int y = radius;
*              int d = 1 – radius;
*
*              while (x <= y) {
*                  plot(mx + x, my + y);
*                  plot(mx + x, my - y);
*
*                  plot(mx - x, my + y);
*                  plot(mx - x, my - y);
*
*                  plot(mx + y, my + x);
*                  plot(mx + y, my - x);
*
*                  plot(mx - y, my + x);
*                  plot(mx - y, my - x);
*
*                  if (d < 0) {
*                      d = d + 2 * x + 3;
*                      x += 1;
*                  } else {
*                      d = d + 2 * (x-y) + 5;
*                      x += 1;
*                      y -= 1;
*                  }
*              }
*          }
*
*          Important! Your code CANNOT import any additional library, so you
*          can implement the absolute values using ternaries and NOT stdlib!
*
*          Important! The pseudocode talks about a "signum" function. Signum is
*          Latin for sign, and the function is simply used to get the sign of an
*          integer. Therefore signum(x) is equal to:
*               -1 if x < 0,
*               0 if x = 0,
*               1 if x > 0.
*
*          You therefore also need to use a ternary for this since such a func.
*          is not avaiable for you to use without using the standard libraries.
*/
void drawCircle(Screen *screen, Circle *circle);

/**
* @brief Draw the filled circle provided, onto the screen provided.
* @param screen The Screen to draw onto.
* @param circle The Circle to draw.
* @details To draw a filled circle, you will need to use an algorithm named
*          Bresenham's Circle Drawing algorithm, which is explained below in
*          pseudocode:
*
*          bresenhamFilledCircle(int mx, int my, int radius) {
*              int x = 0;
*              int y = radius;
*              int d = 1 – radius;
*
*              while (x <= y) {
*                  p1 = (mx + x, my + y);
*                  p2 = (mx + x, (my > y) ? (my - y) : 0);
*                  plot_line(p1, p2);
*
*                  if (mx >= x) {
*                      p3 = (mx - x, my + y);
*                      p4 = (mx - x, (my > y) ? my - y : 0);
*                      plot_line(p3, p4);
*                  }
*
*                  p5 = (mx + y, my + x);
*                  p6 = (mx + y, (my > x) ? (my - x) : 0);
*                  plot_line(p5, p6);
*
*                  if (mx >= y) {
*                      p7 = (mx - y, my + x);
*                      p8 = (mx - y, (my > x) ? (my - x) : 0);
*                      plot_line(p7, p8);
*                  }
*
*                  if (d < 0) {
*                      d = d + 2 * x + 3;
*                      x += 1;
*                  } else {
*                      d = d + 2 * (x-y) + 5;
*                      x += 1;
*                      y -= 1;
*                  }
*              }
*          }
*
*          Important! Your code CANNOT import any additional library, so you
*          can implement the absolute values using ternaries and NOT stdlib!
*
*          Important! The pseudocode talks about a "signum" function. Signum is
*          Latin for sign, and the function is simply used to get the sign of an
*          integer. Therefore signum(x) is equal to:
*               -1 if x < 0,
*               0 if x = 0,
*               1 if x > 0.
*
*          You therefore also need to use a ternary for this since such a func.
*          is not avaiable for you to use without using the standard libraries.
*/
void drawFilledCircle(Screen *screen, Circle *circle);

/**
* @brief Draw the image provided, onto the screen provided, using the provided
*        crop window and filter.
* @param screen The Screen to draw onto.
* @param image The Image to draw.
* @param cropWindow The CropWindow to crop the image with.
* @param colorFilter The color filter function that you will pass each pixel of
*                    the image through, before drawing it.
* @details To draw the image, you will repeat the procedure you used for
*          drawFilledRectangle, but instead of drawing the same color repeatedly
*          you will draw the filtered version of the corresponding pixel from
*          the image provided.
*
*          However, to make this function more flexible, we want to give it the
*          ability to draw just a section of the
*          source image, which will be identified by the cropWindow. The
*          cropWindow contains the position (x, y) of the image where you should
*          start getting pixels from (i.e. the cropWindow top_left pixel will be
*          the first pixel to draw, and it will be drawn at the top_left pixel
*          of the screen provided in the image parameter). You will also get the
*          width and height of the cropWindow and only draw so many pixels from
*          the image, starting at the crop window's top_left point.
*
*          NOTE: THE DRAWN PIXELS SHOULD STILL APPEAR ON THE SCREEN AT THE IMAGE
*          STRUCT'S TOP_LEFT COORDINATES ONWARDS! THE CROP WINDOW ONLY CHANGES
*          WHAT PIXELS YOU RETRIEVE FROM THE IMAGE, BUT THESE DIFFERENT PIXELS
*          SHOULD NOW APPEAR AT THE ORIGIN OF THE IMAGE ON THE SCREEN. LOOK AT
*          THE TESTS IF YOU AREN'T SURE WHAT THIS MEANS.
*
*          A small hint: write the code as though you would draw the entire
*          image. The cropWindow only changes the starting and ending points of
*          your coordinate iteration loops. Instead of starting at the top_left
*          of the image and drawing a box the size of the image, change your
*          loops such that you start at the top_left of the crop window and draw
*          a box the size of the cropWindow.
*
*          Things to consider:
*              * Each pixel obtained from the image needs to be passed through
*                the filter function before being drawn.
*              * If no crop window is provided (if cropWindow == NULL) then you
*                can assume that the entire image will be drawn, and thus create
*                a CropWindow of the full image size, then proceeding normally.
*              * You need to make sure that if the crop window isn't larger than
*                the image or exceeding its bounds in any way, you perform the
*                crop within the bounds of the image, disregarding areas of the
*                crop window outside the image bounds.
*/
void drawImage(Screen *screen, Image *image, CropWindow *cropWindow,
                 Pixel (*colorFilter)(Pixel));
