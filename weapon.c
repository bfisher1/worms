#include "weapon.h"
#include "game.h"
#include <stdlib.h>
#include "physObj.h"
#include <time.h>
#include <math.h>
#include "level.h"
#include "Queue/queue.h"
#include "item.h"
#define DYNAMITE_WIDTH 5
#define DYNAMITE_HEIGHT 20
#define DYNAMITE_DELAY 3
#define DYNAMITE_EXPLOSION_RADIUS 100
#define BLOW_TORCH_DIST 20
#define BLOW_TORCH_RADIUS 20
#define CROSS_HAIR_DIST_SCALE 1.0
#define BULLET_VEL 10
#define BULLET_RANGE 400
#define BULLET_EXPLOSION_RADIUS 20
#define BULLET_START_DIST 40
#define DYNAMITE_EXTRA_HEIGHT 15

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
                      bool fireAtRelease,
                      void (*drawInHand)(Weapon *weapon),
                      void (*activateWeapon)(void *game, int x,
                      int y, float velocity, float orientation) ) {
    weapon->name = name;
    weapon->fireAtRelease = fireAtRelease;
    weapon->drawInHand = drawInHand;
    weapon->activateWeapon = activateWeapon;
}

Weapon *createDynamiteWeapon(Game *game) {
    Weapon *weapon = createWeapon();
    fillWeaponFields(weapon, dynamite, true, (void *) game, activateDynamite);
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

void drawWeapon(WeaponName name, int x, int y, int *frame, void *game, bool firing, bool flippedHoriz, float weaponDir) {
    Game *g = (Game *) game;
    
    switch(name) {
        case grenade:
            break;
        case mine:
            if(playAnim(g->animBank[mineOffAnim], x, y, weaponDir, frame, flippedHoriz)){
                *frame = 0;
            }
            break;
        case dynamite:
            if(playAnim(g->animBank[dynamiteAnim], x, y, weaponDir, frame, flippedHoriz)) {
                *frame = 0;
            }
            break;
        case parachute:
            break;
        case pistol:
            if(playAnim(g->animBank[pistolAnim], x, y, weaponDir, frame, flippedHoriz)){
                *frame = 0;
            }
            break;
        case blowTorch:
            if(!firing){
                if(playAnim(g->animBank[blowTorchStill], x, y, weaponDir, frame, flippedHoriz)){
                    *frame = 0;
                }
            } else {
                if(playAnim(g->animBank[blowTorchFire], x, y, weaponDir, frame, flippedHoriz)){
                    *frame = 0;
                }
            }
            
            break;
    }
}

void fireWeapon(WeaponName name, void *game, int x, int y, float direction, float force) {
    Game *g = (Game *) game;
    int firex, firey;
    float dif;
    Dynamite *d;
    Bullet *b;
    switch(name) {
        case grenade:
            break;
        case mine:
            break;
        case dynamite:
            d = createDynamite(x, y - DYNAMITE_EXTRA_HEIGHT, DYNAMITE_EXPLOSION_RADIUS, DYNAMITE_DELAY, ((Game *) game) );
            d->item->obj->velocity = force;
            d->item->obj->direction = direction;
            enqueue(g->items, d );
            break;
        case parachute:
            break;
        case pistol:
            firex = x + BULLET_START_DIST * cos(direction);
            firey = y + BULLET_START_DIST * sin(direction);
            b = createBullet(firex, firey, BULLET_EXPLOSION_RADIUS, (Game *) game, BULLET_RANGE, direction, BULLET_VEL);
            enqueue(g->items, b);
            break;
        case blowTorch:
            if(direction > 3.14/2 && direction < 3 * 3.14 / 2){
                dif =  -15 * 3.14/180;
            } else {
                dif = 15 * 3.14/180;
            }
            
            firex = x + BLOW_TORCH_DIST * cos(direction - dif);
            firey = y + BLOW_TORCH_DIST * sin(direction - dif);
            cutCircleInLevel(g->level, firex, firey, BLOW_TORCH_RADIUS);
            break;
    }
    
}
void drawCrossHair(void *game, int cx, int cy, float angle, float force) {
    Game *g = (Game *) game;
    int firex, firey, zero;
    firex = cx + CROSS_HAIR_DIST_SCALE * force * cos(angle);
    firey = cy + CROSS_HAIR_DIST_SCALE * force * sin(angle);
    zero = 0;
    playAnim(g->animBank[crossHair], firex, firey, 0, &zero, false);
}
void clearCrossHair(void *game, int cx, int cy, float angle, float force) {
    Game *g = (Game *) game;
    int width, height, firex, firey;
    width = g->animBank[crossHair]->width;
    height = g->animBank[crossHair]->height;
    firex = cx + CROSS_HAIR_DIST_SCALE * force * cos(angle);
    firey = cy + CROSS_HAIR_DIST_SCALE * force * sin(angle);
    drawLevel(g->level, (firex - width) / 8, (firey - height) / 8,
    (firex + width), (firey + height) ); 
}

Weapon *makeWeapons(int len, ...) {
    Weapon *weapons = (Weapon *) malloc(sizeof(Weapon) * len);
    WeaponName name;
    va_list args;
    va_start(args, len);
    for(int i = 0; i < len; i++){
        name = va_arg(args, WeaponName);
        weapons[i].name = name;
        switch(name) {
            case grenade:
                weapons[i].fireAtRelease = true;
                weapons[i].rangedAttack = true;
                break;
            case mine:
                weapons[i].fireAtRelease = true;
                weapons[i].rangedAttack = false;
                break;
            case dynamite:
                weapons[i].fireAtRelease = true;
                weapons[i].rangedAttack = true;
                break;
            case parachute:
                weapons[i].fireAtRelease = true;
                weapons[i].rangedAttack = false;
                break;
            case pistol:
                weapons[i].fireAtRelease = true;
                weapons[i].rangedAttack = false;
                break;
            case blowTorch:
                weapons[i].fireAtRelease = false;
                weapons[i].rangedAttack = false;
                break;
        }
    }
    va_end(args);
    return weapons;
}
void freeWeapons(Weapon *weapons) {
    free(weapons);
}

//void continueWeapon