#include "worm.h"
#include "util.h"
#include "level.h"
#define WORM_WIDTH 20
#define WORM_HEIGHT 20
#define HEALTH_BAR_THICKNESS 4
#define HEALTH_BAR_SCALE .25

Worm *createWorm(char *name, float x, float y, int health, Color *teamColor, Anim *anim, void *team) {
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
    worm->team = team;
    worm->lastPlayed = clock();
    return worm;
}

void drawWormHealthBar(Worm *worm, int thickness) {
    //Color green = {0, 255, 0};
    Color red = {255, 0, 0};
    int x, y, h, health;
    x = worm->obj->x;
    y = worm->obj->y;
    h = worm->currentAnim->height;
    health = worm->health;
    drawRect(worm->currentAnim->screen, x - (MAX_HEALTH / 2) * HEALTH_BAR_SCALE, y - h, health * HEALTH_BAR_SCALE, thickness, *worm->teamColor);
    drawRect(worm->currentAnim->screen, x - (MAX_HEALTH / 2) * HEALTH_BAR_SCALE + health * HEALTH_BAR_SCALE, y - h, (MAX_HEALTH - health) * HEALTH_BAR_SCALE, thickness, red);
}

void drawWorm(Worm *worm) {
    Anim *currentAnim = worm->currentAnim;
    float x = worm->obj->x;
    float y = worm->obj->y;
    float rot = worm->obj->rotation;
    playAnim(currentAnim, x, y, rot, &worm->currentFrame, &worm->lastPlayed, worm->facingRight);
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
    int s = 2;
    int s2 = 2;
    int y, h, x, w;
    y = worm->obj->y;
    h = worm->currentAnim->height;
    x = worm->obj->x;
    w = worm->currentAnim->width;
    drawLevel(level, (worm->obj->x - worm->currentAnim->width * s) / 8,
     (y - h * s) / 8,
    (x + w * s2) ,
    (y + h * s2) );  
}