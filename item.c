#include "item.h"
#include "game.h"
#include "util.h"

/**
    Generate items use these widths and heights by default. Different shaped ones use their own.
*/
#define ITEM_WIDTH 20
#define ITEM_HEIGHT 20
#define BULLET_DIM 5
#define BULLET_DAMAGE 20
#define HEALTH_CRATE_HEAL 25



Item *createItem(ItemName name, int x, int y, int explosionRadius,
                 bool (*wormCollide)(void *this, Worm *worm, void *game),
                 Anim *anim, void (*free)(void *self),
                 int (*itemCollide)(void *this, void *other, void *game)) {
    Item *item = (Item *) malloc(sizeof(Item));
    Box *frame = (Box *) malloc(sizeof(Box));
    frame->x = 0;
    frame->y = 0;
    frame->width = ITEM_WIDTH;
    frame->height = ITEM_HEIGHT;
    item->obj = createPhysObj(x, y, frame, 0, 0);
    item->explosionRadius = explosionRadius;
    item->wormCollide = wormCollide;
    item->itemCollide = itemCollide;
    item->anim = anim;
    item->animFrame = 0;
    item->free = free;
    item->name = name;
    return item;
}

void freeItem(Item *item) {
    free(item->obj);
    free(item);
}

//return whether to delete item after collision
bool healthCrateWormCollide(void *this, Worm *worm, void *game) {
    healWorm(worm, HEALTH_CRATE_HEAL);
    return true;
}
bool weaponCrateWormCollide(void *this, Worm *worm, void *game) {
    return true;
}
bool mineWormCollide(void *this, Worm *worm, void *game) {
    return true;
}
bool noCollideEvent(void *this, Worm *worm, void *game) {
    return false;
}

bool bulletCollideWorm(void *this, Worm *worm, void *game) {
    hurtWorm(worm, BULLET_DAMAGE);
    Bullet *bullet = (Bullet *) this;
    worm->obj->y -= 3;
    worm->obj->velocity = bullet->item->obj->velocity;
    worm->obj->direction = bullet->item->obj->direction;
    return true;
}

//item collisions
int bulletCollideItem(void *this, void *other, void *game) {
    Item *bullet = (Item *) this;
    Item *item = (Item *) other;
    cutCircleInLevel( ((Game *) game)->level, item->obj->x, item->obj->y, item->explosionRadius );
    cutCircleInLevel( ((Game *) game)->level, bullet->obj->x, bullet->obj->y, bullet->explosionRadius );
    return ADD_BACK_NO_ITEMS;
}

int noCollideItem(void *this, void *other, void *game) {
    return ADD_BACK_BOTH_ITEMS;
}

HealthCrate *createHealthCrate(int x, int y, int explosionRadius, int healAmount, void *game) {
    HealthCrate *crate = (HealthCrate *) malloc(sizeof(HealthCrate));
    crate->item = createItem(healthCrateItem, x, y, explosionRadius, healthCrateWormCollide, ((Game *) game)->animBank[healthCrate], freeHealthCrate, noCollideItem );
    crate->healAmount = healAmount;
    return crate;
}

WeaponCrate *createWeaponCrate(int x, int y, int explosionRadius, Weapon *weapon, bool isTrap);

void freeMine(void *mine) {
    Mine *m = (Mine *) mine;
    freeItem(m->item);
    free(m);
}

Mine *createMine(int x, int y, int explosionRadius, clock_t start, float delay, void *game) {
    Mine *mine = (Mine *) malloc(sizeof(Mine));
    mine->item = createItem(mineItem, x, y, explosionRadius, mineWormCollide, ((Game *) game)->animBank[mineOffAnim], freeMine, noCollideItem );
    mine->delay = delay;
    mine->triggered = false;
    return mine;
}

void freeHealthCrate(void *healthCrate) {
    HealthCrate *h = (HealthCrate *) healthCrate;
    freeItem(h->item);
    free(h);
}

void freeWeaponCrate(WeaponCrate *weaponCrate);



void drawItem(Item *item) {
    playAnim(item->anim, item->obj->x, item->obj->y, 0, &item->animFrame, false);
}

void clearItem(Item *item, Level *level) {
    int s = 2;
    int s2 = 2;
    drawLevel(level, (item->obj->x - item->anim->width * s) / 8,
     (item->obj->y - item->anim->height * s) / 8,
    (item->obj->x + item->anim->width * s2) ,
    (item->obj->y + item->anim->height * s2) );  
}

void freeDynamite(void *dynamite) {
    Dynamite *d = (Dynamite *) dynamite;
    freeItem(d->item);
    free(d);
}

Dynamite *createDynamite(int x, int y, int explosionRadius, float delay, void *game) {
    Dynamite *dynamite = (Dynamite *) malloc(sizeof(Dynamite));
    dynamite->item = createItem(dynamiteItem, x, y, explosionRadius, noCollideEvent, ((Game *) game)->animBank[dynamiteAnim], freeDynamite, noCollideItem );
    dynamite->delay = delay;
    dynamite->start = clock();
    return dynamite;
}

bool readyToExplode(void *explosive) {
    Dynamite *dynamite = (Dynamite *) explosive;
    return ( (float) (clock() - dynamite->start) ) / CLOCKS_PER_SEC >= dynamite->delay;
}

void freeBullet(void *bullet) {
    Bullet *b = (Bullet *) bullet;
    freeItem(b->item);
    free(b);
}

Bullet *createBullet(int x, int y, int explosionRadius, void *game, int range, float direction, float velocity) {
    Bullet *bullet = (Bullet *) malloc(sizeof(Bullet));
    bullet->item = createItem(bulletItem, x, y, explosionRadius, bulletCollideWorm, ((Game *) game)->animBank[bulletAnim], freeBullet, bulletCollideItem );
    bullet->item->obj->frame->width = BULLET_DIM;
    bullet->item->obj->frame->height = BULLET_DIM;
    bullet->startx = x;
    bullet->starty = y;
    bullet->range = range;
    bullet->item->obj->direction = direction;
    bullet->item->obj->velocity = velocity;
    return bullet;
}

bool bulletOutOfRange(Bullet *bullet) {
    return dist(bullet->startx, bullet->starty, bullet->item->obj->x, bullet->item->obj->y) > bullet->range;
}