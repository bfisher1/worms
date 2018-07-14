#include "team.h"
#include "util.h"

Team *createTeam(char *name, int teamNumber, Color color, Weapon *weapons, int *weaponNums, int weaponNumber, Level *level) {
    Team *team = (Team *) malloc(sizeof(Team));
    team->color = color;
    Worm **worms = (Worm **) malloc(sizeof(Worm *) * teamNumber);
    for(int i = 0; i < teamNumber; i++) {
        worms[i] = createWorm("Worm", randInt(0, level->width), 100, 100, &team->color, NULL);
    }
    team->worms = worms;
    team->name = name;
    team->teamNumber = teamNumber;
    
    team->weapons = weapons;
    team->weaponNums = weaponNums;
    team->weaponNumber = weaponNumber;
    return team;
}

int teamHealth(Team *team) {
    int health = 0;
    for(int i = 0; i < team->teamNumber; i++) {
        health += team->worms[i]->health;
    }
    return health;
}

void freeTeam(Team *team) {
    for(int i = 0; i < team->teamNumber; i++) {
        freeWorm(team->worms[i]);
    }
    free(team);
}