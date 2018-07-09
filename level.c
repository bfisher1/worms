#include "level.h"
#include "util.h"
#define BBP 4

Level *loadLevel(char *foregroundName, char *backgroundName, char *terrainName, SDL_Surface *screen) {

    //Color white = {255, 255, 255};
    Color black = {0, 0, 0};
    Level *level = (Level *) malloc(sizeof(Level));
    level->foreground = loadPPM(foregroundName);
    if(level->foreground == NULL){
        return NULL;
    }
    level->background = loadPPM(backgroundName);
    if(level->background == NULL){
        freeImage(level->foreground);
        return NULL;
    }

    Image *terrainImg = loadPPM(terrainName);

    level->width = terrainImg->width;
    level->height = terrainImg->height;
    level->terrain = (TerrainBlock **) malloc(sizeof(TerrainBlock *) * level->height / 8);
    unsigned char mask;
    for(int i = 0; i < level->height / 8; i++) {
        level->terrain[i] = (TerrainBlock *) malloc(sizeof(TerrainBlock) * level->width);
        for(int j = 0; j < level->width / 8; j++) {
            
            for(int h = 0; h < 8; h++) {
                level->terrain[i][j].blocks[h] = 0x0;
                for(int w = 0; w < 8; w++) {
                    mask = 0x80 >> w;
                    if( sameColors(&terrainImg->grid[i * 8 + h][j * 8 + w], &black)) {
                        level->terrain[i][j].blocks[h] |= mask;
                    }
                }
            }
        }
    }
    freeImage(terrainImg);
    level->screen = screen;
    return level;
}
void drawLevel(Level *level, int x, int y, int xmax, int ymax) {
    unsigned char mask;
    int ytimesw;
    //Color red = {255, 0, 0};
    //Color black = {0, 0, 0};
    for(int i = y; i < ymax / 8; i++) {
        for(int j = x; j < xmax / 8; j++) {
            for(int h = 0; h < 8; h++) {
                ytimesw = (i * 8 + h)*level->screen->pitch/BPP;
                for(int w = 0; w < 8; w++) {
                    //zeroes every where except at widthbinary
                    mask = 0x80 >> w;
                    if(level->terrain[i][j].blocks[h] & mask){
                        setpixel(level->screen, j * 8 + w, ytimesw, level->foreground->grid[i * 8 + h][j * 8 + w]);
                    } else {
                        setpixel(level->screen, j * 8 + w, ytimesw, level->background->grid[i * 8 + h][j * 8 + w]);
                    }

                }
            }
        }
    }
}

void cutCircleInLevel(Level *level, int x, int y, int radius) {
    int xstart, ystart, xmax, ymax, w, h;
    xstart = x - radius;
    ystart = y - radius;
    if(x < 0){
        xstart = 0;
    }
    if(y < 0){
        ystart = 0;
    }
    xmax = x + radius;
    ymax = y + radius;
    if(x > level->width){
        xstart = level->width;
    }
    if(y > level->height){
        ystart = level->height;
    }
    for(int i = ystart; i < ymax; i++) {
        for(int j = xstart; j < xmax; j++) {
            if(dist(x, y, j, i) <= radius) {
                w = j%8;
                h = i%8;
                level->terrain[i/8][j/8].blocks[h] &= ~(0x80 >> w);
            }
        }
    }
}

void freeLevel(Level *level) {
    freeImage(level->foreground);
    freeImage(level->background);
    for(int i = 0; i < level->height / 8; i++) {
        free(level->terrain[i]);
    }
}

bool groundAt(Level *level, int x, int y) {
    return level->terrain[y/8][x/8].blocks[y%8] & (0x80 >> (x%8));
}