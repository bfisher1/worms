/**
    @file util.h
    @author Ben Fisher

    Header file containing declarations for miscellaneous functions,
    such as generating random integers and delaying the program.
*/
#ifndef UTIL
#define UTIL
#include <stdbool.h>
/**
    Generates a random integer between min and max.

    @param min the min random number
    @param max the max random number
    
    @return random number between min and max
*/
int randInt(int min, int max);

/**
    Generates a random float value between min and max.

    @param min the min random value
    @param max the max random value
    
    @return random number between min and max
*/
float randFloat(float min, float max);

/**
    Delays the program for the given amount of milliseconds.

    @param delay the time the program should be delayed for
*/
void delayMs(float delay);

/**
    Calculates the distance between 2 points.

    @param x1 the x coordinate of the first point
    @param y1 the y coordinate of the first point
    @param x2 the x coordinate of the second point
    @param y2 the y coordinate of the second point
    
    @return the distance between the 2 points
*/
float dist(float x1, float y1, float x2, float y2);

/**
    Applies pacman boundaries to cartesian coordinates.
    If coordinates are outside the width or height, they get set to
    0. The opposite is true for being negative.

    @param x the x coordinate that may be changed
    @param y the y coordinate that may be changed
    @param width the width of the pacman boundaries
    @param height the height of the pacman boundaries
    @param boundary how far off this boundaries an item is teleported
*/
void pacmanBounds(float *x, float *y, int width, int height, int boundary);

/**
    Determines if 2 lines intersect.

    @param l1x1 the x coordinate of the first point of the first line
    @param l1y1 the y coordinate of the first point of the first line
    @param l1x2 the x coordinate of the second point of the first line
    @param l1y2 the y coordinate of the second point of the first line
    @param l2x1 the x coordinate of the first point of the second line
    @param l2y1 the y coordinate of the first point of the second line
    @param l2x2 the x coordinate of the second point of the second line
    @param l2y2 the y coordinate of the second point of the second line

    @return true if the lines intersect, false if not
*/
bool lineSegsIntersect(float l1x1, float l1y1, float l1x2, float l1y2, float l2x1, float l2y1, float l2x2, float l2y2);

/**
    Determines if a line segment and a ray intersect.

    @param lx1 the x coordinate of the first point of the line segment
    @param ly1 the y coordinate of the first point of the line segment
    @param lx2 the x coordinate of the second point of the line segment
    @param ly2 the y coordinate of the second point of the line segment
    @param rx1 the x coordinate of the first point of the ray
    @param ry1 the y coordinate of the first point of the ray
    @param rx2 the x coordinate of the second point of the ray
    @param ry2 the y coordinate of the second point of the ray

    @return true if the line and rays intersect, false if not
*/
bool doLineSegAndRayIntersect(float lx1, float ly1, float lx2, float ly2, float rx1, float ry1, float rx2, float ry2);
#endif /* UTIL */