/**
    @file team.h
    @author Ben Fisher

    Defines function for dealing with teams.
*/
#ifndef TEAM_H
#define TEAM_H
#include "worm.h"
#include "graphics.h"
#include "weapon.h"

/**
    Structure of a team. Each team has a name, color, a collection
    of worms, and a weapons inventory.
*/
typedef struct {
    char *name;
    Color color;
    int teamNumber;
    Worm **worms;
    int weaponNumber;
    Weapon *weapons;
    int *weaponNums;
    int selectedWeapon;
} Team;

/**
    Creates a team with the given name, number of members, and color.

    @param name the name of the team
    @param names the names of the team members
    @param teamNumber the number of members on the team
    @param color the team's color
    @param weapons the array of weapons in the team's inventory
    @param weaponNums array of numbers corresponding to the stock of
           each weapon in inventory
    @param weaponNumber the amount of weapons in the inventory
    @param level the level the team is being made in

    @return the team that was created
*/
Team *createTeam(char *name, char **names, int teamNumber, Color color, Weapon *weapons, int *weaponNums, int weaponNumber, Level *level);

/**
    Calculates the team's overall health.

    @param team the team being checked
    
    @return the sum of the health of the team members
*/
int teamHealth(Team *team);

/**
    Frees the given team.

    @param team the team being freed
*/
void freeTeam(Team *team);

void removeWormFromTeam(Team *team, int index);

#endif /* TEAM_H */