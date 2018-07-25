/**
    @file level.h
    @author Ben Fisher

    Defines the level structure and functions for dealing with it.
    LEvels can be drawn, freed, and damaged.
*/
#ifndef LEVEL_H
#define LEVEL_H
#include <stdbool.h>
#include <SDL/SDL.h>
#include "graphics.h"
#define BLOCK_SIZE 8
/**
    Structure for a block of terrain. Each block is 8x8 pixels.
*/
typedef struct {
    unsigned char blocks[8];
} TerrainBlock;

/**
    Structure for a level. A level has dimensions, a background,
    an image that makes up the terrain, a terrain, water level,
    and screen that it gets drawn to.
*/
typedef struct {
    int width;
    int height;
    Image *foreground;
    Image *background;
    TerrainBlock **terrain;
    int waterLevel;
    SDL_Surface *screen;
    int xshift;
    int yshift;
} Level;

/**
    Loads a level with the given foreground, background, and terrain
    file names. All files are ppms currently.

    @param foregroundName the name of the foreground file
    @param backgroundName the name of the background file
    @param terrainName the name of the terrain file
*/
Level *loadLevel(char *foregroundName, char *backgroundName, char *terrainName, SDL_Surface *screen);

/**
    Draws a level starting at (x,y) on the screen. It does not draw
    past xmax and ymax on the screen.

    @param level the level being drawn
    @param x the x coordinate of the level's top left corner
    @param y the y coordinate of the level's top left corner
    @param xmax the boundary where pixels cannot be drawn past
    @param ymax the boundary where pixels cannot be drawn past
*/
void drawLevel(Level *level, int x, int y, int xmax, int ymax);

/**
    Frees a level from memory.

    @param level the level being freed
*/
void freeLevel(Level *level);

/**
    Cuts a circle in the level at (x,y) with the given radius.

    @param level the level being cut
    @param x the x coordinate of the cirle being cut
    @param y the y coordinate of the cirle being cut
    @param radius the radius of the circle being cut in the level
*/
void cutCircleInLevel(Level *level, int x, int y, int radius);

/**
    Determines if there is ground at (x,y) in the level.

    @param level the level being checked
    @param x the x coordinate of the level point
    @param y the y coordinate of the level point
*/
bool groundAt(Level *level, int x, int y);
#endif