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
#include "weapon.h"
#include "team.h"
#include "game.h"
#include "Queue/queue.h"

#define WIDTH 1920
#define HEIGHT 696
#define DEPTH 32
#define REFRESH_RATE .005
#define GRAVITY .4 // .8
#define TERMINAL_FALL_VELOCITY 25.0
#define X_ACCEL .2
#define MAX_X_VELOCITY 5.0
#define JUMP 8.0
#define MAX_JUMP_VELOCITY 25
#define TURN_LENGTH 15
#define INV_SIZE 3
//add suddent death and water

/**
    The program's starting point.

    @param argc number of command line arguments
    @param argv the command line arguments

    @return exit status of the program
*/
int main(int argc, char *argv[])
{ 
    /*
    SDL_Surface *screen;
    SDL_Event event;
    

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
    */
    Color green = {0, 255, 0};
    Color blue = {0, 0, 255};
    Level *level = loadLevel("levels/dad_level_foreground.ppm", "levels/driveway_background.ppm", "levels/dad_level_level.ppm", NULL);
    //loadLevel("levels/dad_level_foreground.ppm", "levels/driveway_background.ppm", "levels/dad_level_level.ppm", NULL);
    //loadLevel("levels/ben_level_foreground.ppm", "levels/driveway_background.ppm", "levels/ben_level_level.ppm", NULL);
    //Worm *worm; // = createWorm("Springy", 202, 111, 100, &green, animBank[wormMove]);
    
    char *teamOneNames[] = {"Firefox", "BST", "Hidden", "Blender", "PuTTY", "Huffman", "Seg fault"};
    Weapon *teamOneWeapons = makeWeapons(INV_SIZE, parachute, mine, grenade);
    int weaponNumsOne[] = {1, 0, 3};
    Team *teamOne = createTeam("Annihilators", teamOneNames, 7, green, teamOneWeapons, weaponNumsOne, INV_SIZE, level);

    char *teamTwoNames[] = {"FSM", "C++", "Python", "Exception", "HTML", "Hash table", "Assembly"};
    Weapon *teamTwoWeapons = makeWeapons(INV_SIZE, blowTorch, dynamite, pistol);
    int weaponNumsTwo[] = {2, 8, 1};
    Team *teamTwo = createTeam("OverClockers", teamTwoNames, 7, blue, teamTwoWeapons, weaponNumsTwo, INV_SIZE, level);

    //Team *teams[]  = {teamOne, teamTwo};
    Queue *teams = makeQueue();
    enqueue(teams, teamOne);
    enqueue(teams, teamTwo);
    
    /*
    int teamNumber = 2;
    for(int i = 0; i < teamNumber; i++) {
        for(int j = 0; j < teams[i]->teamNumber; j++) {
            switchAnim(teams[i]->worms[j], animBank[wormStill]);
        }
    }
    
    Worm *selectedWorm = teams[0]->worms[0];
    */
    //Image *redBackground = loadPPM("levels/red_background.ppm");
    
    /*
    time_t t;
    srand((unsigned) time(&t));
    */

    Game *game = startGame(level, teams, TURN_LENGTH, GRAVITY);
    level->screen = game->screen;
    drawLevel(game->level, 0, 0, WIDTH, HEIGHT);
    enqueue(game->items, createHealthCrate(426, 376, 60, 100, (void *) game));
    enqueue(game->items, createHealthCrate(466, 376, 60, 100, (void *) game));
    enqueue(game->items, createHealthCrate(516, 376, 60, 100, (void *) game));
    
    //Image *life = loadPPM("levels/level1.ppm");
    
    //Level *level = loadLevel("levels/tiny_foreground.ppm", "levels/tiny_background.ppm", "levels/tiny level.ppm", screen);

    //Color black = {0, 0, 0};
    
    
    bool gameOver = false;
    /*
    int mousex, mousey;
    bool cut = false;
    bool left = false;
    bool right = false;
    bool up = false;
    int c = 0;
    //float dir;
    bool explode = false;
    drawLevel(level, 0, 0, WIDTH, HEIGHT);
    clock_t lastUpdate = clock();
    bool le, ri, to, bo;
    */
	while(!gameOver) {
        gameOver = gameLoop(game);
        /*
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
            isColliding(selectedWorm->obj, level, &le, &ri, &to, &bo);
            if(right) {
                switchAnim(selectedWorm, animBank[wormMove]);
                if(to){
                    accel(selectedWorm->obj, selectedWorm->obj->rotation, .8, .8);
                } else {
                    selectedWorm->obj->x += .4;
                }
                //printf("%f\n", worm->obj->rotation);
                
                //moveRight(worm->obj,level, 3);
                flipWormRight(selectedWorm);
            }
            if(left) {
                switchAnim(selectedWorm, animBank[wormMove]);
                //accel(worm->obj, PI, X_ACCEL, MAX_X_VELOCITY);
                if(to) {
                    accel(selectedWorm->obj, selectedWorm->obj->rotation + PI, .8, .8);
                } else {
                    selectedWorm->obj->x -= .4;
                }
                
                //moveLeft(worm->obj,level, 3);
                flipWormLeft(selectedWorm);
            }
            if(up && to) {
                //jump(worm->obj, level, 7);
                //printf("JUMPING\n");
                accel(selectedWorm->obj,  -PI/2.0, JUMP, MAX_JUMP_VELOCITY);
            }
            if(!right && !left) {
                decel(selectedWorm->obj, X_ACCEL);
                switchAnim(selectedWorm, animBank[wormStill]);
                if(selectedWorm->facingRight) {
                    flipWormRight(selectedWorm);
                } else {
                    flipWormLeft(selectedWorm);
                }
            }

            for(int i = 0; i < teamNumber; i++) {
                for(int j = 0; j < teamOne->teamNumber; j++) {
                    worm = teams[i]->worms[j];
                    isColliding(worm->obj, level, &le, &ri, &to, &bo);
                    if(!bo) {
                        accel(worm->obj, PI/2.0, GRAVITY, TERMINAL_FALL_VELOCITY);
                    }
                    
                    //printf("vvl %f\n", worm->obj->velocity);
                    
                    if(cut){
                        cutCircleInLevel(level, mousex, mousey, 50);
                        cut = false;
                    }
                    if(explode) {
                        //cutCircleInLevel(level, worm->obj->x, worm->obj->y, 50);
                        //float dir = PI + atan2(mousey - worm->obj->y, mousex - worm->obj->x);
                        //float dir = -PI / 2;
                        //accel(worm->obj,  dir, 20, 150);
                        hurtWorm(worm, randInt(12, 30));
                        
                    }

                    
                    move(worm->obj, level, 0);
                    tilt(worm->obj, level, 2 * PI/180.0, screen);
                    //drawLevel(level, 0, 0, WIDTH, HEIGHT);
                    
                }
            }
            
            explode = false;
            //printf("%d %d %d %d\n", le, ri, to, bo);
            */
            
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
            //drawWorm(worm);
            /*
            for(int i = 0; i < teamNumber; i++) {
                for(int j = 0; j < teams[i]->teamNumber; j++) {
                    drawWorm(teams[i]->worms[j]);
                }
            }
            */
            //drawWorm(teams[0]->worms[0]);
            
            //updateScreen(screen);
            //lastUpdate = clock();
            /*
            for(int i = 0; i < teamNumber; i++) {
                for(int j = 0; j < teams[i]->teamNumber; j++) {
                    clearWorm(teams[i]->worms[j], level);
                }
            }
            */
            
        //}
        
	}
    endGame(game);
    freeWeapons(teamOneWeapons);
    freeWeapons(teamTwoWeapons);
    /*
    freeImage(redBackground);
    freeLevel(level);
    freeAnims(animBank, animBankLen);
    //freeWorm(worm);
    freeTeam(teamOne);
    freeTeam(teamTwo);
    SDL_Quit();
    */
    
	return 0;
}