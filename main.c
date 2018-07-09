/**
    @file main.c
    @author Ben Fisher

    Eventually will be a turn based game of worms.
*/
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "graphics.h"
#include "util.h"
#include <time.h>
#include "level.h"

#define WIDTH 1000
#define HEIGHT 600
#define DEPTH 32
#define PI 3.14
#define REFRESH_RATE .01

/**
    The program's starting point.

    @param argc number of command line arguments
    @param argv the command line arguments

    @return exit status of the program
*/
int main(int argc, char *argv[])
{ 
    
    float x, y;
    x = 100;
    y = 100;
    SDL_Surface *screen;
    SDL_Event event;
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
   
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_RESIZABLE|SDL_HWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }

    time_t t;
    srand((unsigned) time(&t));

    //Image *life = loadPPM("levels/level1.ppm");
    Level *level = loadLevel("levels/level1_foreground.ppm", "levels/level1_background.ppm", "levels/level1.ppm", screen);

    //Color black = {0, 0, 0};
    //Color red = {255, 0, 0};
    Color green = {0, 255, 0};
    bool gameOver = false;
    int mousex, mousey;
    bool cut = false;
    
    clock_t lastUpdate = clock();
	while(!gameOver) {
        while(SDL_PollEvent(&event)) 
         {
             SDL_GetMouseState(&mousex, &mousey);
              switch (event.type) 
              {
                  case SDL_QUIT:
                    gameOver = true;
                    break;
                  case SDL_KEYDOWN:
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT:
                            break;
                        case SDLK_RIGHT:
                            break;
                        case SDLK_UP:
                            cut = true;
                            break;
                        case SDLK_DOWN:
                            break;
                        case SDLK_ESCAPE:
                            gameOver = true;
                            break;
                        case SDLK_SPACE:
                            x = mousex;
                            y = mousey;
                            break;
                        default:
                            break;
                    }
                    break;  
                  case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT:
                            break;
                        case SDLK_RIGHT:
                            break;
                        case SDLK_UP:
                            cut = false;
                            break;
                        case SDLK_DOWN:
                            break;
                        default:
                            break;
                    }
                    break;
              }
         }
        if(cut){
            cutCircleInLevel(level, mousex, mousey, 50);
            cut = false;
        }
        //drawRect(screen, 0, 0, WIDTH, HEIGHT, black);
        if( ( (float) (clock() - lastUpdate)) / CLOCKS_PER_SEC >= REFRESH_RATE ){
            
            drawLevel(level, 0, 0, WIDTH, HEIGHT);
            //drawImage(level->foreground, screen, 0, 0);
            //drawRect(screen, mousex, mousey, 100, 100, red);
            drawRect(screen, x, y, 10, 10, green);
            //printf("%d %d\n", x, y);
            updateScreen(screen);
            lastUpdate = clock();
        }
        if(!groundAt(level, x, y)){
            y += .001;
        }
	}

    freeLevel(level);
    SDL_Quit();
    
	return 0;
}