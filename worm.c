#include "worm.h"
#include "util.h"
#include "level.h"
#define WORM_WIDTH 20
#define WORM_HEIGHT 20
#define HEALTH_BAR_THICKNESS 4
#define HEALTH_BAR_SCALE .25

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
    worm->currentFrame = 0;
    return worm;
}

void drawWormHealthBar(Worm *worm, int thickness) {
    Color green = {0, 255, 0};
    Color red = {255, 0, 0};
    drawRect(worm->currentAnim->screen, worm->obj->x - (MAX_HEALTH / 2) * HEALTH_BAR_SCALE, worm->obj->y - worm->currentAnim->height, worm->health * HEALTH_BAR_SCALE, thickness, green);
    drawRect(worm->currentAnim->screen, worm->obj->x - (MAX_HEALTH / 2) * HEALTH_BAR_SCALE + worm->health * HEALTH_BAR_SCALE, worm->obj->y - worm->currentAnim->height, (MAX_HEALTH - worm->health) * HEALTH_BAR_SCALE, thickness, red);
    
}

void drawWorm(Worm *worm) {
    Anim *currentAnim = worm->currentAnim;
    float x = worm->obj->x;
    float y = worm->obj->y;
    float rot = worm->obj->rotation;
    playAnim(currentAnim, x, y, rot, &worm->currentFrame);
    drawWormHealthBar(worm, HEALTH_BAR_THICKNESS);
}

void switchAnim(Worm *worm, Anim *anim) {
    if(worm->currentAnim != anim) {
        worm->currentFrame = 0;
        worm->currentAnim = anim;
    }
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

void clearWorm(Worm *worm, Level *level) {
    drawLevel(level, (worm->obj->x - worm->currentAnim->width) / 8,
     (worm->obj->y - worm->currentAnim->height) / 8,
    (worm->obj->x + worm->currentAnim->width) ,
    (worm->obj->y + worm->currentAnim->height) );  
}