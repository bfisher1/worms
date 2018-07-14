/**
    @file graphics.h
    @author Ben Fisher

    Contains functions dealing with graphics, such as drawing a line
    or a rectangle to a screen. Much of the SDL code is based on code
    from http://www.friedspace.com/cprogramming/sdlbasic.php.
*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL/SDL.h>
#include <stdbool.h>
#define BPP 4
typedef struct {
    Uint8 r;
    Uint8 g;
    Uint8 b;
} Color;

typedef struct {
    int width;
    int height;
    Color **grid;
} Image;

//#define WHITE (Color) { .r = 255, .g = 255, .b = 255 }

/**
    Sets a pixel to the given color. Most of the code is
    from http://friedspace.com/SDLTest.c.

    @param screen the screen the pxiel is being set on
    @param x the x coordinate of the pixel
    @param y the y coordinate of the pixel
    @param color the color the pixel is set to
*/
void setpixel(SDL_Surface *screen, int x, int y, Color color );

/**
    Draws a line defined by 2 points to a screen.
    The algorithm used is the Digital differential analyzer.
    Based on code from:
    https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)

    @param screen the screen that the line is being drawn to
    @param x1 the x coordinate of the first point defining the line
    @param y1 the y coordinate of the first point defining the line
    @param x2 the x coordinate of the second point defining the line
    @param y2 the y coordinate of the second point defining the line
    @param color the color of the line
*/
void drawLine(SDL_Surface* screen, int x1, int y1, int x2, int y2, Color color);

/**
    Draws a rectangle defined by a point, width, and height to
    the given screen.

    @param screen the screen that the rectangle is being drawn to
    @param x the x coordinate of the rectangle's upper left-hand corner
    @param y the y coordinate of the rectangle's upper left-hand corner
    @param width the width of the rectangle
    @param height the height of the rectangle
    @param color the color of the rectangle
*/
void drawRect(SDL_Surface* screen, int x, int y, int width, int height, Color color);

/**
    Updates the given screen.

    @param screen the screen being updated
*/
void updateScreen(SDL_Surface* screen);

/**
    Draws a circle to the given screen. Uses code and algorithm from
    https://en.wikipedia.org/wiki/Midpoint_circle_algorithm.

    @param screen the screen that the circle is being drawn on
    @param x0 the x coordinate at the center of the circle
    @param y0 the y coordinate at the center of the circle
    @param r the radius of the circle being drawn
    @param color the color being drawn
*/
void drawCircle(SDL_Surface* screen, int x0, int y0, int r, Color color);

/**
    Loads a ppm file into an image.

    @param filename the name of the ppm file being read from
    
    @return the image in the file
*/
Image *loadPPM(char *fileName);

/**
    Draws an image to the given screen, offset by x and y.

    @param img the image being drawn
    @param screen the screen the image is being drawn on
    @param x the amount of horizontal offset the image is rendered at
    @param y the amount of vertical offset the image is rendered at
*/
void drawImage(Image *img, SDL_Surface *screen, int x, int y);

/**
    Frees an image from memory.

    @param img the image being freed.
*/
void freeImage(Image *img);

/**
    Determines if 2 colors are the same.

    @param col1 the first color
    @param col2 the second color
    
    @return true if the colors are the same, false if they are not
*/
bool sameColors(Color *col1, Color *col2);

/**
    Draws a rectangular section of the image with the top left corner
    at (x,y) on the screen. Bad things happen when angle passes 90.

    @param img the image whose subsection is being drawn
    @param screen the screen the subimage is being drawn on
    @param x the x coordinate of the center of the drawn subimage
    @param y the y coordinate of the center of the drawn subimage
    @param rx the x coordinate of the left corner of the subimage
           rectangle relative to the image
    @param ry the y coordinate of the left corner of the subimage
           rectangle relative to the image
    @param rwidth the width of the subimage rectangle
    @param rheight the height of the subimage rectangle
    @param background the background color that gets skipped when drawn
           NULL for the background will fill in the whole subimage
    @param flippedHoriz whether or not the image is flipped horizontally
    @param angle the angle the image is rotated at
*/
void drawSubImage(Image *img, SDL_Surface *screen, int centerx, int centery, int rx, int ry, int rwidth, int rheight, Color *background, bool flippedHoriz, float angle);

void plotpix(SDL_Surface *screen, int x, int y, Color color );
#endif /* GRAPHICS_H */