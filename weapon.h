/**
    @file weapon.h
    @author Ben Fisher

    Defines structs and functions for dealing with weapons.
*/
#ifndef WEAPON_H
#define WEAPON_H

/**
    Enumeration for the names of weapons.
*/
typedef enum {
    grenade,
    mine,
    dynamite,
    parachute,
    pistol
} WeaponName;

/**
    Weapon struct. Contains a function
    for its effect in game.
*/
typedef struct WeaponTag {
    WeaponName name;
    void (*drawInHand)(struct WeaponTag *weapon);
    void (*activateWeapon)(void *game, int x, int y, float velocity, float orientation);
} Weapon;

Weapon *createDynamiteWeapon();
Weapon *createMineWeapon();
Weapon *createPistolWeapon();

void drawWeapon(WeaponName name, int x, int y, int *frame, void *game);

void freeWeapon(Weapon *weapon);

void fireWeapon(WeaponName name, void *game, int x, int y, float direction, float velocity);

#endif /* WEAPON_H */
