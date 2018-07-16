/**
    @file game.h
    @author Ben Fisher

    Defines the game structure and functions related to events in
    the game. The game keeps track of everything in the game.
    These include the level and lists of worms and items.
    It is easier to manage these entities in game rather than in
    main.
*/
#ifndef GAME_H
#define GAME_H
#include "worm.h"
#include "item.h"
#include "Queue/queue.h"
#include "team.h"
#include "anim.h"
#include <stdbool.h>
#include <SDL/SDL.h>
#include "graphics.h"
#include <time.h>
#include "text.h"

/**
    Structure for a game. A game contains references to all
    items and worms in it. A game has a set gravity and length that
    each team's turn can be. It has a flag for if the player can
    change team members.
*/
typedef struct {
    Level *level;
    Queue *teams;
    Queue *items;
    Anim **animBank;
    int animNumber;
    float gravity;
    bool teamCanSwitchMember;
    int turnLength;
    SDL_Surface *screen;
    clock_t lastUpdate;
    Worm *player;
    Team *currentTeam;
    Font *font;
    Queue *stamps;
} Game;

/**
    Begins a game with the given level, teams, turn length, and gravity.

    @param level the game's level
    @param teams the teams in the game
    @param turn length each player's turn length
    @param gravity the gravity in the game
    
    @return the game made from these parameters
*/
Game *startGame(Level *level, Queue *teams, int turnLength, float gravity);

/**
    Moves all physical objects in the game.

    @param game the game whose objects are being moved
*/
void moveObjectsInGame(Game *game);

/**
    Figures out if the worms have settled after the weapon has gone off.

    @param game the game worms are being checked in
*/
bool wormsHaveSettled(Game *game);

/**
    Frees a game from memory.

    @param game the game being freed.
*/
void endGame(Game *game);

bool gameLoop(Game *game);
#endif /* GAME_H */