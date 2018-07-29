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
    static int id = 0;
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
    item->id = id++;
    item->lastPlayed = clock();
    return item;
}

void freeItem(Item *item) {
    free(item->obj);
    free(item);
}

void triggerMine(Game *game, Mine *mine) {
    if(!mine->triggered) {
        mine->triggered = true;
        mine->start = clock();
        mine->item->anim = game->animBank[mineBlinkAnim];
    }
}

//return whether to delete item after collision
bool healthCrateWormCollide(void *this, Worm *worm, void *game) {
    healWorm(worm, HEALTH_CRATE_HEAL);
    return true;
}
bool weaponCrateWormCollide(void *this, Worm *worm, void *game) {
    Weapon *weapon = randWeapon((Game *) game);
    giveWormWeapon((Game *) game, worm, weapon);
    return true;
}
bool mineWormCollide(void *this, Worm *worm, void *game) {
    //mine -> triggered = true;
    //mine current anim = blinking
    //velocity isn't 0
    if( (int) ((Mine *) this)->item->obj->velocity ) {
        return false;
    }
    //mine is still
    triggerMine((Game *) game, (Mine *) this);
    return false;
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
    Item *bullet = ((ItemSubclass *) this)->item;
    Item *item = ((ItemSubclass *) other)->item;
    createExplosion((Game *) game, item->obj->x, item->obj->y, item->explosionRadius);
    createExplosion((Game *) game, bullet->obj->x, bullet->obj->y, bullet->explosionRadius);
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

WeaponCrate *createWeaponCrate(int x, int y, int explosionRadius, Weapon *weapon, bool isTrap, void *game) {
    WeaponCrate *crate = (WeaponCrate *) malloc(sizeof(WeaponCrate));
    crate->item = createItem(weaponCrateItem, x, y, explosionRadius, weaponCrateWormCollide, ((Game *) game)->animBank[weaponCrate], freeWeaponCrate, noCollideItem );
    crate->weapon = weapon;
    crate->isTrap = isTrap;
    return crate;
}

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

void freeWeaponCrate(void *weaponCrate) {
    WeaponCrate *w = (WeaponCrate *) weaponCrate;
    freeItem(w->item);
    freeWeapon(w->weapon);
    free(w);
}



void drawItem(Item *item) {
    if(item->name == mineItem){
        //printf("%d %d\n", &item->animFrame, item->animFrame);
    }
    if(playAnim(item->anim, item->obj->x, item->obj->y, 0, &item->animFrame, &item->lastPlayed, false)){
        //item->animFrame = 0;
    }
}

void clearItem(Item *item, Level *level) {
    int s = 2;
    int s2 = 2;
    int x, y, w, h;
    x = item->obj->x;
    y = item->obj->y;
    w = item->anim->width;
    h = item->anim->height;
    drawLevel(level, (x - w * s) / 8,
     (y - h * s) / 8,
    (x + w * s2) ,
    (y + h * s2) );  
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

bool dynamiteReadyToExplode(void *explosive) {
    Dynamite *dynamite = (Dynamite *) explosive;
    return ( (float) (clock() - dynamite->start) ) / CLOCKS_PER_SEC >= dynamite->delay;
}

bool mineReadyToExplode(Mine *explosive) {
    Mine *mine = (Mine *) explosive;
    return mine->triggered && ( (float) (clock() - mine->start) ) / CLOCKS_PER_SEC >= mine->delay;
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