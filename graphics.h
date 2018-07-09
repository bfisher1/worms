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

Image *loadPPM(char *fileName);
void drawImage(Image *img, SDL_Surface *screen, int x, int y);
void freeImage(Image *img);

bool sameColors(Color *col1, Color *col2);
#endif /* GRAPHICS_H */