#include "item.h"
#include "game.h"

/**
    Generate items use these widths and heights by default. Different shaped ones use their own.
*/
#define ITEM_WIDTH 20
#define ITEM_HEIGHT 20
Item *createItem(int x, int y, int explosionRadius, void (*wormCollide)(Worm *worm, void *game), Anim *anim) {
    Item *item = (Item *) malloc(sizeof(Item));
    Box *frame = (Box *) malloc(sizeof(Box));
    frame->x = 0;
    frame->y = 0;
    frame->width = ITEM_WIDTH;
    frame->height = ITEM_HEIGHT;
    item->obj = createPhysObj(x, y, frame, 0, 0);
    item->explosionRadius = explosionRadius;
    item->wormCollide = wormCollide;
    item->anim = anim;
    item->animFrame = 0;
    return item;
}

void freeItem(Item *item) {
    free(item->obj);
    free(item);
}

void healthCrateWormCollide(Worm *worm, void *game) {
    //
}
void weaponCrateWormCollide(Worm *worm, void *game) {
    //
}
void mineWormCollide(Worm *worm, void *game) {
    //
}
void noCollideEvent(Worm *worm, void *game) {
    //
}

HealthCrate *createHealthCrate(int x, int y, int explosionRadius, int healAmount, void *game) {
    HealthCrate *crate = (HealthCrate *) malloc(sizeof(HealthCrate));
    crate->item = createItem(x, y, explosionRadius, healthCrateWormCollide, ((Game *) game)->animBank[healthCrate] );
    crate->healAmount = healAmount;
    return crate;
}

WeaponCrate *createWeaponCrate(int x, int y, int explosionRadius, Weapon *weapon, bool isTrap);

Mine *createMine(int x, int y, int explosionRadius, clock_t start, float delay);

void freeHealthCrate(HealthCrate *healthCrate) {
    freeItem(healthCrate->item);
    free(healthCrate);
}

void freeWeaponCrate(WeaponCrate *weaponCrate);

void freeMine(Mine *mine);

void drawItem(Item *item) {
    playAnim(item->anim, item->obj->x, item->obj->y, 0, &item->animFrame);
}

void clearItem(Item *item, Level *level) {
    drawLevel(level, (item->obj->x - item->anim->width) / 8,
     (item->obj->y - item->anim->height) / 8,
    (item->obj->x + item->anim->width) ,
    (item->obj->y + item->anim->height) );  
}