/**
    @file weapon.h
    @author Ben Fisher

    Defines structs and functions for dealing with weapons.
*/
#ifndef WEAPON_H
#define WEAPON_H
//#include "game.h"
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
typedef struct {
    WeaponName name;
    //void (*effect)(Game *game);
} Weapon;

#endif /* WEAPON_H */
