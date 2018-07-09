/**
    @file item.h
    @author Ben Fisher

    Defines the structure for items, such as crates and health boxes.
*/
#ifndef ITEM_H
#define ITEM_H
#include "physObj.h"
#include "worm.h"
#include "level.h"
#include "weapon.h"

/**
    An item has a physical object component, a current animation, and
    a function for if it collides with a worm in the level.
*/
typedef struct {
    PhysObj *obj;
    Anim *currentAnim;
    void (*wormCollide)(Worm *worm, Level *level);
    int explosionRadius;
} Item;

/**
    Structure for health crate.
*/
typedef struct {
    Item *item;
    int healAmount;
} HealthCrate;

/**
    Structure for a weapon crate.
*/
typedef struct {
    Item *item;
    bool isTrap;
    Weapon *weapon;
} WeaponCrate;

/**
    Structure for a mine.
*/
typedef struct {
    Item *item;
} Mine;

/**
    Creates a health crate with coordinates (x,y) and an explosion
    radius.

    @param x the start x coordinate of the crate
    @param y the start y coordinate of the crate
    @param explosionRadius the radius of the explosion if the crate
           explodes
    
    @return the created health crate
*/
HealthCrate *createHealthCrate(int x, int y, int explosionRadius);

/**
    Creates a health crate at coordinates (x,y), an explosion
    radius, and the weapon inside.

    @param x the start x coordinate of the crate
    @param y the start y coordinate of the crate
    @param explosionRadius the radius of the explosion if the crate
           explodes
    
    @return the created weapon crate
*/
WeaponCrate *createWeaponCrate(int x, int y, int explosionRadius, Weapon *weapon);

/**
    Creates a mine at (x,y) with the given explosion radius.

    @param x the start x coordinate of the mine
    @param y the start y coordinate of the mine
    @param explosionRadius the radius of the explosion if the mine
           explodes
    
    @return the created mine
*/
Mine *createMine(int x, int y, int explosionRadius);

/**
    Frees the health crate.

    @param healthCrate the crate being freed
*/
void freeHealthCrate(HealthCrate *healthCrate);

/**
    Frees the weapon crate.

    @param weaponCrate the crate being freed
*/
void freeWeaponCrate(WeaponCrate *weaponCrate);

/**
    Frees the mine.

    @param mine the mine being freed
*/
void freeMine(Mine *mine);
#endif /* ITEM_H */