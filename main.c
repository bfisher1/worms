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
#include "anim.h"
#include "worm.h"
#include <math.h>

#define WIDTH 1000
#define HEIGHT 800
#define DEPTH 32
#define REFRESH_RATE .005
#define GRAVITY .4 // .8
#define TERMINAL_FALL_VELOCITY 25.0
#define X_ACCEL .2
#define MAX_X_VELOCITY 5.0
#define JUMP 8.0
#define MAX_JUMP_VELOCITY 25

/**
    The program's starting point.

    @param argc number of command line arguments
    @param argv the command line arguments

    @return exit status of the program
*/
int main(int argc, char *argv[])
{ 
    SDL_Surface *screen;
    SDL_Event event;
    Color green = {0, 255, 0};
	if (SDL_Init(SDL_INIT_VIDEO) < 0 ) return 1;
   
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_RESIZABLE|SDL_HWSURFACE)))
    {
        SDL_Quit();
        return 1;
    }

    int animBankLen;
    Anim **animBank = loadAnims(screen, &animBankLen);

    if(!animBank) {
        SDL_Quit();
        fprintf(stderr, "Couldn't load animation\n");
        return EXIT_FAILURE;
    }

    Worm *worm = createWorm("Springy", 202, 111, 100, &green, animBank[wormMove]);

    time_t t;
    srand((unsigned) time(&t));

    //Image *life = loadPPM("levels/level1.ppm");
    Level *level = loadLevel("levels/level1_foreground.ppm", "levels/level1_background.ppm", "levels/level3.ppm", screen);
    //Level *level = loadLevel("levels/tiny_foreground.ppm", "levels/tiny_background.ppm", "levels/tiny level.ppm", screen);

    //Color black = {0, 0, 0};
    
    
    bool gameOver = false;
    int mousex, mousey;
    bool cut = false;
    bool left = false;
    bool right = false;
    bool up = false;
    int c = 0;
    float dir;
    bool explode = false;
    clock_t lastUpdate = clock();
	while(!gameOver) {
        c++;
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
                            left = true;
                            break;
                        case SDLK_RIGHT:
                            right = true;
                            break;
                        case SDLK_UP:
                            up = true;
                            break;
                        case SDLK_DOWN:
                            explode = true;
                            //worm->obj->direction = -PI / 2.0;
                            //worm->obj->velocity = 10;
                            break;
                        case SDLK_ESCAPE:
                            gameOver = true;
                            break;
                        case SDLK_SPACE:
                            cut = true;
                            break;
                        default:
                            break;
                    }
                    break;  
                  case SDL_KEYUP:
                    switch(event.key.keysym.sym){
                        case SDLK_LEFT:
                            left = false;
                            break;
                        case SDLK_RIGHT:
                            right = false;
                            break;
                        case SDLK_UP:
                            up = false;
                            break;
                        case SDLK_DOWN:
                            break;
                        case SDLK_SPACE:
                            cut = false;
                            break;
                        default:
                            break;
                    }
                    break;
              }
         }
        if(c == 400000){
            c = 0;
        }
        if( ( (float) (clock() - lastUpdate)) / CLOCKS_PER_SEC >= REFRESH_RATE ) {
            //worm->obj->rotation += .01;
            bool le, ri, to, bo;
            printf(" el %f\n", worm->obj->velocity);
            isColliding(worm->obj, level, &le, &ri, &to, &bo);
            if(!bo) {
                accel(worm->obj, PI/2.0, GRAVITY, TERMINAL_FALL_VELOCITY);
            }
            printf("vvl %f\n", worm->obj->velocity);
            
            if(cut){
                cutCircleInLevel(level, mousex, mousey, 50);
                cut = false;
            }
            if(explode) {
                cutCircleInLevel(level, worm->obj->x, worm->obj->y, 50);
                dir = PI + atan2(mousey - worm->obj->y, mousex - worm->obj->x);
                accel(worm->obj,  dir, 20, 150);
                explode = false;
            }

            if(right) {
                switchAnim(worm, animBank[wormMove]);
                if(to){
                    accel(worm->obj, worm->obj->rotation, 2, 15);
                } else {
                    worm->obj->x += 2;
                }
                //printf("%f\n", worm->obj->rotation);
                
                //moveRight(worm->obj,level, 3);
                flipWormRight(worm);
            }
            if(left) {
                switchAnim(worm, animBank[wormMove]);
                //accel(worm->obj, PI, X_ACCEL, MAX_X_VELOCITY);
                if(to) {
                    accel(worm->obj, worm->obj->rotation + PI, 2, 15);
                } else {
                    worm->obj->x -= 2;
                }
                
                //moveLeft(worm->obj,level, 3);
                flipWormLeft(worm);
            }
            if(up && to) {
                //jump(worm->obj, level, 7);
                printf("JUMPING\n");
                accel(worm->obj, -PI/2.0, JUMP, MAX_JUMP_VELOCITY);
            }
            if(!right && !left) {
                decel(worm->obj, X_ACCEL);
                switchAnim(worm, animBank[wormStill]);
                if(worm->facingRight) {
                    flipWormRight(worm);
                } else {
                    flipWormLeft(worm);
                }
            }
            //moveVert(worm->obj, level);
            //drawRect(screen, 0, 0, WIDTH, HEIGHT, black);
            //moveDown(worm->obj, level, 10);
            //fall(worm->obj, GRAVITY, TERMINAL_FALL_VELOCITY);
            
        printf("%d %d %d %d\n", le, ri, to, bo);
            //printf("%f %f \n", worm->obj->direction, worm->obj->velocity);
            // printf("yyyy %f \n", worm->obj->y);
            
            //printf("-----\n");
            //printf("YYY %f\n", worm->obj->y);
            if(worm->obj->frame == NULL) {
                printf("~~~~~~~~~~~\n");
            }
            //printf("wdith %d\n", worm->obj->frame->width);
            //printf("vel %f\n", worm->obj->velocity);
            
            move(worm->obj, level, 0);
            tilt(worm->obj, level, PI/180.0, screen);
            drawLevel(level, 0, 0, WIDTH, HEIGHT);
            //printf("orient %f\n", worm->obj->rotation);
            //printf("++++++\n");
            /*
            Color red = {255, 0, 0};
            
            float x, y, rad, dir;
            rad = dist(0, 0, worm->obj->frame->width, worm->obj->frame->height);
            dir = worm->obj->rotation;
            x = worm->obj->x + worm->obj->frame->x;
            y = worm->obj->y + worm->obj->frame->y;
            
            drawLine(screen, x, y, x + rad * cos(dir), y + rad * sin(dir), red);
            drawLine(screen, x, y, x + rad * cos(dir - PI / 2.0), y + rad * sin(dir - PI / 2.0), green);
            drawRect(screen, worm->obj->x + worm->obj->frame->x - worm->obj->frame->width / 2.0, worm->obj->y + worm->obj->frame->y - worm->obj->frame->height / 2.0, worm->obj->frame->width, worm->obj->frame->height, red);
            */
            drawWorm(worm);

            
            updateScreen(screen);
            lastUpdate = clock();
        }
	}

    freeLevel(level);
    freeAnims(animBank, animBankLen);
    freeWorm(worm);
    SDL_Quit();
    
	return 0;
}