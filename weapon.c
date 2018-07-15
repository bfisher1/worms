#include "weapon.h"
#include "game.h"
#include <stdlib.h>
#include "physObj.h"
#include <time.h>
#include "level.h"
#include "Queue/queue.h"
#include "item.h"
#define DYNAMITE_WIDTH 5
#define DYNAMITE_HEIGHT 20
#define DYNAMITE_DELAY 3
#define DYNAMITE_EXPLOSION_RADIUS 100

/**
typedef struct WeaponTag {
    WeaponName name;
    void (*drawInHand)(struct WeaponTag *weapon);
    void (*activateWeapon)(void *game, int x, int y, float velocity, float orientation);
    void (*continueWeapon)(void *game);
} Weapon;
*/

void activateDynamite(void *game, int x, int y, float velocity, float orientation) {
    static bool created = false;
    static bool finished = false;
    static PhysObj *dynamite = NULL;
    static clock_t start;
    if(!created) {
        Box *frame = (Box *) malloc(sizeof(Box));
        frame->x = 0;
        frame->y = 0;
        frame->width = DYNAMITE_WIDTH;
        frame->height = DYNAMITE_HEIGHT;
        dynamite = createPhysObj(x, y, frame, velocity, orientation);
        created = true;
        start = clock();
    }

    if(created && !finished) {
        //draw dynamite
        Game *game = (Game *) game;
        if( ((float) (clock() - start)) / CLOCKS_PER_SEC >= DYNAMITE_DELAY ) {
            finished = true;
            cutCircleInLevel(game->level, dynamite->x, dynamite->y, DYNAMITE_EXPLOSION_RADIUS);
        }
    }

    if(finished) {
        freePhysObj(dynamite);
        created = false;
        finished = false;
        dynamite = NULL;
    }
}


Weapon *createWeapon() {
    return (Weapon *) malloc(sizeof(Weapon));
}

void fillWeaponFields(Weapon *weapon, WeaponName name,
                      void (*drawInHand)(Weapon *weapon),
                      void (*activateWeapon)(void *game, int x,
                      int y, float velocity, float orientation) ) {
    weapon->name = name;
    weapon->drawInHand = drawInHand;
    weapon->activateWeapon = activateWeapon;
}

Weapon *createDynamiteWeapon(Game *game) {
    Weapon *weapon = createWeapon();
    fillWeaponFields(weapon, dynamite, (void *) game, activateDynamite);
    return weapon;
}

Weapon *createMineWeapon(){
    Weapon *weapon = createWeapon();
    return weapon;
}

Weapon *createPistolWeapon() {
    Weapon *weapon = createWeapon();
    return weapon;
}

void freeWeapon(Weapon *weapon) {
    free(weapon);
}

void drawWeapon(WeaponName name, int x, int y, int *frame, void *game) {
    Game *g = (Game *) game;
    switch(name) {
        case grenade:
            break;
        case mine:
            break;
        case dynamite:
            if(playAnim(g->animBank[dynamiteAnim], x, y, 0, frame)) {
                *frame = 0;
            }
            break;
        case parachute:
            break;
        case pistol:
            break;
    }
}

void fireWeapon(WeaponName name, void *game, int x, int y, float direction, float velocity) {
    cutCircleInLevel( ((Game *) game)->level, x, y, 20 );
    cutCircleInLevel( ((Game *) game)->level, x+40, y, 40 );
    //enqueue((Game *game)->items, );
}

//void continueWeapon