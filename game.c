#include "game.h"
#include <stdlib.h>
#include "util.h"
#define WIDTH 1000
#define HEIGHT 800
#define DEPTH 32
#define TERMINAL_FALL_VELOCITY 25.0
#define REFRESH_RATE .005
#define X_ACCEL .2
#define MAX_X_VELOCITY 5.0
#define JUMP 8.0
#define MAX_JUMP_VELOCITY 25

void readKeys(SDL_Event *event, bool *left, bool *right, bool *up, bool *down, bool *space, bool *tab, bool *esc);

Game *startGame(Level *level, Queue *teams, int turnLength, float gravity) {
    SDL_Surface *screen;
    if (SDL_Init(SDL_INIT_VIDEO) < 0 ) exit(EXIT_FAILURE);
   
    if (!(screen = SDL_SetVideoMode(WIDTH, HEIGHT, DEPTH, SDL_RESIZABLE|SDL_HWSURFACE)))
    {
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
    
    int animBankLen;
    Anim **animBank = loadAnims(screen, &animBankLen);

    if(!animBank) {
        SDL_Quit();
        fprintf(stderr, "Couldn't load animation\n");
        exit(EXIT_FAILURE);
    }

    time_t t;
    srand((unsigned) time(&t));
    Game *game = (Game *) malloc(sizeof(Game));
    game->level = level;
    game->teams = teams;
    game->items = makeQueue();
    game->animBank = animBank;
    game->animNumber = animBankLen;
    game->gravity = gravity;
    game->turnLength = turnLength;
    game->teamCanSwitchMember = false;
    if(!game->animBank) {
        exit(EXIT_FAILURE);
    }
    game->screen = screen;
    Team *team;
    for(int i = 0; i < queueSize(game->teams); i++) {
        team = dequeue(game->teams);
        enqueue(game->teams, team);
        for(int j = 0; j < team->teamNumber; j++) {
            switchAnim(team->worms[j], game->animBank[wormStill]);
        }
    }
    game->player = team->worms[0];
    game->lastUpdate = clock();
    return game;
}

void moveObjectsInGame(Game *game);

bool wormsHaveSettled(Game *game);

void endGame(Game *game) {
    freeAnims(game->animBank, game->animNumber);
    SDL_Quit();
    freeLevel(game->level);
    int teamNumber = queueSize(game->teams);
    for(int i = 0; i < teamNumber; i++) {
        Team *team = dequeue(game->teams);
        freeTeam(team);
    }
    int itemNumber = queueSize(game->items);
    for(int i = 0; i < itemNumber; i++) {
        //freeItem
    }
    freeQueue(game->items);
    freeQueue(game->teams);
    free(game);
}

bool gameLoop(Game *game) {
    static SDL_Event event;
    static bool left, right, up, down, space, tab, esc, le, ri, to, bo;
    static int mousex, mousey, teamNumber, wormNumber, itemNumber;
    static Worm *worm;
    static Team *team;
    readKeys(&event, &left, &right, &up, &down, &space, &tab, &esc);
    if(esc) {
        return true;
    }
    SDL_GetMouseState(&mousex, &mousey);

    teamNumber = queueSize(game->teams);
    
    if( ( (float) (clock() - game->lastUpdate)) / CLOCKS_PER_SEC >= REFRESH_RATE ) {

        isColliding(game->player->obj, game->level, &le, &ri, &to, &bo);
        if(right) {
            switchAnim(game->player, game->animBank[wormMove]);
            if(to){
                accel(game->player->obj, game->player->obj->rotation, .8, .8);
            } else {
                game->player->obj->x += .4;
            }
            //printf("%f\n", worm->obj->rotation);
            
            //moveRight(worm->obj,level, 3);
            flipWormRight(game->player);
        }
        if(left) {
            switchAnim(game->player, game->animBank[wormMove]);
            //accel(worm->obj, PI, X_ACCEL, MAX_X_VELOCITY);
            if(to) {
                accel(game->player->obj, game->player->obj->rotation + PI, .8, .8);
            } else {
                game->player->obj->x -= .4;
            }
            
            //moveLeft(worm->obj,level, 3);
            flipWormLeft(game->player);
        }
        if(up && to) {
            accel(game->player->obj,  -PI/2.0, JUMP, MAX_JUMP_VELOCITY);
        }
        if(!right && !left) {
            decel(game->player->obj, X_ACCEL);
            switchAnim(game->player, game->animBank[wormStill]);
            if(game->player->facingRight) {
                flipWormRight(game->player);
            } else {
                flipWormLeft(game->player);
            }
        }


        for(int i = 0; i < teamNumber; i++) {
            team = dequeue(game->teams);
            enqueue(game->teams, team);
            wormNumber = team->teamNumber;
            for(int j = 0; j < wormNumber; j++) {
                
                worm = team->worms[j];
                //do stuff with worms
                //accel
                //move
                //draw
                isColliding(worm->obj, game->level, &le, &ri, &to, &bo);
                if(!bo) {
                    accel(worm->obj, PI/2.0, game->gravity, TERMINAL_FALL_VELOCITY);
                }
                move(worm->obj, game->level, 0);
                tilt(worm->obj, game->level, 2 * PI/180.0, 45.0 * PI / 180.0,  game->screen );
                drawWorm(worm);
            }
        }
        itemNumber = queueSize(game->items);
        for(int i = 0; i < itemNumber; i++) {
            //do item stuff
        }
        updateScreen(game->screen);
        for(int i = 0; i < teamNumber; i++) {
            team = dequeue(game->teams);
            enqueue(game->teams, team);
            wormNumber = team->teamNumber;
            for(int j = 0; j < wormNumber; j++) {
                worm = team->worms[j];
                clearWorm(worm, game->level);
            }
        }
        
    }
    return false;

}

void readKeys(SDL_Event *event, bool *left, bool *right, bool *up, bool *down, bool *space, bool *tab, bool *esc) {
    while(SDL_PollEvent(event)) {
        
        switch (event->type) 
        {
            case SDL_QUIT:
                break;
            case SDL_KEYDOWN:
                switch(event->key.keysym.sym){
                    case SDLK_LEFT:
                        *left = true;
                        break;
                    case SDLK_RIGHT:
                        *right = true;
                        break;
                    case SDLK_UP:
                        *up = true;
                        break;
                    case SDLK_DOWN:
                        *down = true;
                        break;
                    case SDLK_ESCAPE:
                        *esc = true;
                        break;
                    case SDLK_SPACE:
                        *space = true;
                        break;
                    case SDLK_TAB:
                        *tab = true;
                    default:
                        break;
                }
                break;  
            case SDL_KEYUP:
                switch(event->key.keysym.sym) {
                    case SDLK_LEFT:
                        *left = false;
                        break;
                    case SDLK_RIGHT:
                        *right = false;
                        break;
                    case SDLK_UP:
                        *up = false;
                        break;
                    case SDLK_DOWN:
                        *down = false;
                        break;
                    case SDLK_ESCAPE:
                        *esc = false;
                        break;
                    case SDLK_SPACE:
                        *space = false;
                        break;
                    case SDLK_TAB:
                        *tab = false;
                    default:
                        break;
                }
                break;
        }
    }
}