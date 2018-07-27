#include "team.h"
#include "util.h"

Team *createTeam(char *name, char **names, int teamNumber, Color color, ArrayList *weapons,  Level *level) {
    Team *team = (Team *) malloc(sizeof(Team));
    team->color = color;
    Worm **worms = (Worm **) malloc(sizeof(Worm *) * teamNumber);
    for(int i = 0; i < teamNumber; i++) {
        worms[i] = createWorm(names[i], randInt(0, level->width), 0, 100, &team->color, NULL);
    }
    team->worms = worms;
    team->name = name;
    team->teamNumber = teamNumber;
    
    team->weapons = weapons;
    team->selectedWeapon = 0;
    team->playerIdx = 0;
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

void removeWormFromTeam(Team *team, int index) {
    if(team->teamNumber <= 0) {
        return;
    }
    freeWorm(team->worms[index]);
    team->teamNumber--;
    for(int i = index; i < team->teamNumber; i++) {
        team->worms[i] = team->worms[i + 1];
    }
}