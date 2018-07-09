/**
    @file graphics.c
    @author Ben Fisher

    Defines functions dealing with graphics, such as drawing a line
    or a rectangle to a screen.
*/
#include "graphics.h"
#include <stdlib.h>
#include <stdio.h>
#include "level.h"

void setpixel(SDL_Surface *screen, int x, int y, Color color ) {
    int yInv = y * BPP / screen->pitch;
    if(0 <= x && x < screen->w && 0 <= yInv && yInv < screen->h){
        Uint32 *pixmem32;
        Uint32 colour;  
    
        colour = SDL_MapRGB( screen->format, color.r, color.g, color.b );
        
        pixmem32 = (Uint32*) screen->pixels  + y + x;
        *pixmem32 = colour;
    }
}

void drawLine(SDL_Surface* screen, int x1, int y1, int x2, int y2, Color color) {

    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0) return;
    }
    
    float dx = x2 - x1;
    float dy = y2 - y1;
    float x, y, step;
    int ytimesw;
    if(abs(dx) >= abs(dy)){
        step = abs(dx);
    } else {
        step = abs(dy);
    }

    dx /= step;
    dy /= step;
    
    x = x1;
    y = y1;
    for (int i = 1; i <= step; i++) {
        ytimesw = ((int) y)*screen->pitch/BPP;
        if(0 <= x && x < screen->w && 0 <= y && y < screen->h) {
            setpixel(screen, (int) x, (int) ytimesw, color); 
        }
        x += dx;
        y += dy;
    }

    
}

void drawRect(SDL_Surface* screen, int x, int y, int width, int height, Color color) {
    #include <stdio.h>
    int ytimesw;
  
    if(SDL_MUSTLOCK(screen)) 
    {
        if(SDL_LockSurface(screen) < 0) return;
    }
    int lastX = x + width;
    int lastY = y + height;
    int startX = x;
    int startY = y;
    for(int x = startX; x < lastX; x++) {
        for(int y = startY; y < lastY; y++) {
            ytimesw = y*screen->pitch/BPP;
            if(0 <= x && x < screen->w && 0 <= y && y < screen->h){
                setpixel(screen, x, ytimesw, color); 
            }
            //printf("%d %d\n", x, ytimesw);
        }
    }

}

void updateScreen(SDL_Surface* screen) {
    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
  
    SDL_Flip(screen); 
}

void drawCircle(SDL_Surface* screen, int x0, int y0, int r, Color color) {
    int x = r - 1;
    int y = 0;
    int dx = 1;
    int dy = 1;
    int err = dx - (r << 1);
    while (x >= y){
        setpixel(screen, x0 + x, (y0 + y) * screen->pitch/BPP, color);
        setpixel(screen, x0 + y, (y0 + x) * screen->pitch/BPP, color);
        setpixel(screen, x0 - y, (y0 + x) * screen->pitch/BPP, color);
        setpixel(screen, x0 - x, (y0 + y) * screen->pitch/BPP, color);
        setpixel(screen, x0 - x, (y0 - y) * screen->pitch/BPP, color);
        setpixel(screen, x0 - y, (y0 - x) * screen->pitch/BPP, color);
        setpixel(screen, x0 + y, (y0 - x) * screen->pitch/BPP, color);
        setpixel(screen, x0 + x, (y0 - y) * screen->pitch/BPP, color);
        
        if (err <= 0){
            y++;
            err += dy;
            dy += 2;
        }

        if (err > 0){
            x--;
            dx += 2;
            err += dx - (r << 1);
        }
    }

}

Image *loadPPM(char *fileName) {
    FILE *f = fopen(fileName, "r");
    if(f == NULL){
        fclose(f);
        return NULL;
    }
    fscanf(f, "P3");
    Image *img = (Image *) malloc(sizeof(Image));
    fscanf(f, "%d %d", &img->width, &img->height);
    int colorMax;
    fscanf(f, "%d", &colorMax);
    img->grid = (Color **) malloc(sizeof(Color *) * img->height);
    int r, g, b;
    
    for(int i = 0; i < img->height; i++) {
        img->grid[i] = (Color *) malloc(sizeof(Color) * img->width);
        for(int j = 0; j < img->width; j++) {
            fscanf(f, "%d", &r);
            fscanf(f, "%d", &g);
            fscanf(f, "%d", &b);
            
            img->grid[i][j].r = r;
            img->grid[i][j].g = g;
            img->grid[i][j].b = b;
        }
    }
    //first line
    //if 
    fclose(f);
    return img;
}
void drawImage(Image *img, SDL_Surface *screen, int x, int y) {
    for(int i = 0; i < img->width; i++) {
        if( !(0 <= y && y < screen->h) ) {
            break;
        }
        for(int j = 0; j < img->height; j++){
            int ytimesw = (j+y)*screen->pitch/BPP;
            if( !(0 <= x && x < screen->w) ) {
                break;
            }
                setpixel(screen, i+x, ytimesw, img->grid[j][i]);
        }
        
        
    }
}
void freeImage(Image *img) {
    for(int i = 0; i < img->height; i++) {
        free(img->grid[i]);
    }
    free(img->grid);
    free(img);
}

bool sameColors(Color *col1, Color *col2) {
    return col1->r == col2->r && col1->g == col2->g && col1->b == col2->b;
}
