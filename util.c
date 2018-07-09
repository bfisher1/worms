/**
    @file util.c
    @author Ben Fisher

    Implementation file containing definitions for miscellaneous functions,
    such as generating random integers and delaying the program.
*/
#include "util.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>

int randInt(int min, int max){
    return rand() % (max - min) + min;
}

float randFloat(float min, float max) {
    return cos(rand()) * (max - min) + min;
}

void delayMs(float delay) {
    clock_t start = clock();
    while ( 1000.0 * ((float) (clock() - start)) / CLOCKS_PER_SEC <= delay );
}

float dist(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1;
    float dy = y2 - y1;
    return sqrt( dx * dx + dy * dy );
}

void pacmanBounds(float *x, float *y, int width, int height, int boundary){
    if(*x < -boundary){
        *x = width;
    }
    if(*x > width + boundary){
        *x = 0;
    }
    if(*y < -boundary){
        *y = height;
    }
    if(*y > height + boundary){
        *y = 0;
    }
}
/**TODO CONSIDER VERTICAL CASES
*/
bool lineSegsIntersect(float l1x1, float l1y1, float l1x2, float l1y2, float l2x1, float l2y1, float l2x2, float l2y2) {
    float denominator = (l1x1 - l1x2)*(l2y1 - l2y2) - (l1y1 - l1y2) * (l2x1 - l2x2);
    if(denominator != 0) {
        //Lines are not parallel
        float xnum = (l1x1*l1y2 - l1y1*l1x2)*(l2x1 - l2x2) - (l1x1 - l1x2)*(l2x1*l2y2 - l2y1*l2x2);
        float ynum = (l1x1*l1y2 - l1y1*l1x2)*(l2y1 - l2y2) - (l1y1 - l1y2)*(l2x1*l2y2 - l2y1*l2x2);

        //Coordinates of where two lines would intersect
        float xintersect = xnum / denominator;
        float yintersect = ynum / denominator;

        //Checks that the intersection occurs on the line segment
        float l1xrange[2];
        float l2xrange[2];
        if(l1x1 <= l1x2) {            
            l1xrange[0] = l1x1;
            l1xrange[1] = l1x2;
        } else {
            l1xrange[0] = l1x2;
            l1xrange[1] = l1x1;
        }

        if(l2x1 <= l2x2) {            
            l2xrange[0] = l2x1;
            l2xrange[1] = l2x2;
        } else {
            l2xrange[0] = l2x2;
            l2xrange[1] = l2x1;
        }

        float l1yrange[2];
        float l2yrange[2];
        if(l1y1 <= l1y2) {            
            l1yrange[0] = l1y1;
            l1yrange[1] = l1y2;
        } else {
            l1yrange[0] = l1y2;
            l1yrange[1] = l1y1;
        }

        if(l2y1 <= l2y2) {            
            l2yrange[0] = l2y1;
            l2yrange[1] = l2y2;
        } else {
            l2yrange[0] = l2y2;
            l2yrange[1] = l2y1;
        }
        bool xInRange = l1xrange[0] <= xintersect && xintersect <= l1xrange[1] && l2xrange[0] <= xintersect && xintersect <= l2xrange[1];
        bool yInrange = l1yrange[0] <= yintersect && yintersect <= l1yrange[1] && l2yrange[0] <= yintersect && yintersect <= l2yrange[1];
        return xInRange && yInrange;
    }
    return false;
}

bool doLineSegAndRayIntersect(float lx1, float ly1, float lx2, float ly2, float rx1, float ry1, float rx2, float ry2) {
    float denominator = (lx1 - lx2)*(ry1 - ry2) - (ly1 - ly2) * (rx1 - rx2);
    if(denominator != 0) {
        //Lines are not parallel
        float xnum = (lx1*ly2 - ly1*lx2)*(rx1 - rx2) - (lx1 - lx2)*(rx1*ry2 - ry1*rx2);
        float ynum = (lx1*ly2 - ly1*lx2)*(ry1 - ry2) - (ly1 - ly2)*(rx1*ry2 - ry1*rx2);

        //Coordinates of where two lines would intersect
        float xintersect = xnum / denominator;
        float yintersect = ynum / denominator;

        //Checks that the intersection occurs on the line segment
        float l1xrange[2];
        if(lx1 <= lx2) {            
            l1xrange[0] = lx1;
            l1xrange[1] = lx2;
        } else {
            l1xrange[0] = lx2;
            l1xrange[1] = lx1;
        }
        float lyrange[2];
        if(ly1 <= ly2) {            
            lyrange[0] = ly1;
            lyrange[1] = ly2;
        } else {
            lyrange[0] = ly2;
            lyrange[1] = ly1;
        }
        bool xInRange;
        bool yInRange;

        bool rayGoesUp = ry1 <= ry2;
        if(rayGoesUp){
            yInRange = lyrange[0] <= yintersect && yintersect <= lyrange[1] && yintersect >= ry1;
        } else {
            yInRange = lyrange[0] <= yintersect && yintersect <= lyrange[1] && yintersect <= ry1;
        }
        bool rayGoesRight = rx1 <= rx2;
        if(rayGoesRight){
            xInRange = l1xrange[0] <= xintersect && xintersect <= l1xrange[1] && xintersect >= rx1;
        } else {
            xInRange = l1xrange[0] <= xintersect && xintersect <= l1xrange[1] && xintersect <= rx1;
        }
        return xInRange && yInRange;  
    }
    return false;
}