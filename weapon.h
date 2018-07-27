/**
    @file weapon.h
    @author Ben Fisher

    Defines structs and functions for dealing with weapons.
*/
#ifndef WEAPON_H
#define WEAPON_H
#include <stdbool.h>
#include <stdarg.h>
#define WEAPON_NUMBER 6 
#include "List/ArrayList.h"

/**
    Enumeration for the names of weapons.
*/
typedef enum {
    grenade,
    mine,
    dynamite,
    parachute,
    pistol,
    blowTorch
} WeaponName;

/**
    Weapon struct. Contains a function
    for its effect in game.
*/
typedef struct WeaponTag {
    WeaponName name;
    bool fireAtRelease;
    bool rangedAttack;
    void (*drawInHand)(struct WeaponTag *weapon);
    void (*activateWeapon)(void *game, int x, int y, float velocity, float orientation);
} Weapon;

typedef struct {
    Weapon weapon;
    int amount;
} InvWeapon;

Weapon *createDynamiteWeapon();
Weapon *createMineWeapon();
Weapon *createPistolWeapon();

void drawWeapon(WeaponName name, int x, int y, int *frame, void *game, bool firing, bool flippedHoriz, float weaponDir);

void freeWeapon(Weapon *weapon);

void fireWeapon(WeaponName name, void *game, int x, int y, float direction, float velocity);

void drawCrossHair(void *game, int cx, int cy, float angle, float force);

void clearCrossHair(void *game, int cx, int cy, float angle, float force);

ArrayList *makeWeapons(int len, ...);
void freeWeapons(ArrayList *weapons);
Weapon *randWeapon(void *game);
#endif /* WEAPON_H */
