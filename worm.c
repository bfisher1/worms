#include "worm.h"
#include "util.h"
#define WORM_WIDTH 20
#define WORM_HEIGHT 20

Worm *createWorm(char *name, float x, float y, int health, Color *teamColor, Anim *anim) {
    Worm *worm = (Worm *) malloc(sizeof(Worm));
    worm->name = name;
    Box *frame = (Box *) malloc(sizeof(Box));
    frame->x = 0;
    frame->y = 0;
    frame->width = WORM_WIDTH;
    frame->height = WORM_HEIGHT;
    worm->obj = createPhysObj(x, y, frame, 0.0, 0.0);
    worm->health = health;
    worm->teamColor = teamColor;
    worm->facingRight = randInt(0,2);
    worm->currentAnim = anim;
    return worm;
}

void drawWorm(Worm *worm) {
    Anim *currentAnim = worm->currentAnim;
    float x = worm->obj->x;
    float y = worm->obj->y;
    float rot = worm->obj->rotation;
    playAnim(currentAnim, x, y, rot);
}

void switchAnim(Worm *worm, Anim *anim) {
    worm->currentAnim = anim;
}

void flipWormRight(Worm *worm) {
    worm->facingRight = true;
    worm->currentAnim->flippedHoriz = true;
}

void flipWormLeft(Worm *worm) {
    worm->facingRight = false;
    worm->currentAnim->flippedHoriz = false;
}

void hurtWorm(Worm *worm, int damage) {
    worm->health -= damage;
}

void healWorm(Worm *worm, int healingFactor) {
    worm->health += healingFactor;
}

void freeWorm(Worm *worm) {
    freePhysObj(worm->obj);
    free(worm);
}