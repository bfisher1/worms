/**
    @file worm.h
    @author Ben Fisher

    Definition file for the worm structure and functions related to
    it. A worm contains a physical object to control its movement and
    physics. It also has health, current animation, a name, and a
    team color. Facing right corresponds to the direction the worm
    is facing. Contains the current anim's frame.
*/
#ifndef WORM_H
#define WORM_H
#include "physObj.h"
#include "anim.h"
#include "graphics.h"
#include <time.h>
#define MAX_HEALTH 100
/**
    Structure for a worm.
*/
typedef struct {
    PhysObj *obj;
    int health;
    Anim *currentAnim;
    Color *teamColor;
    char *name;
    bool facingRight;
    int currentFrame;
    void *team;
    clock_t lastPlayed;
} Worm;

/**
    Creates a worm at (x, y) with the given team and health.

    @param name the worm's name
    @param x the worm's starting x coordinate
    @param y the worm's starting y coordinate
    @param health the worm's starting health
    @param color the worm's team color
    @param anim the worm's starting anim

    @return the created worm
*/
Worm *createWorm(char *name, float x, float y, int health, Color *teamColor, Anim *anim, void *team);

/**
    Draws the worm according to its current animation.

    @param worm the worm being drawn
*/
void drawWorm(Worm *worm);

/**
    Switches the worm's current animation to another one.

    @param worm the worm having its animation switched.
    @param anim the new animation the worm will play
*/
void switchAnim(Worm *worm, Anim *anim);

/**
    Flips the direction of the worm left.

    @param worm the worm turning direction
*/
void flipWormLeft(Worm *worm);

/**
    Flips the direction of the worm right.

    @param worm the worm turning direction
*/
void flipWormRight(Worm *worm);

/**
    Lowers the worm's health by the given damage.

    @param worm the worm losing health
    @param damage the amount of health the worm loses
*/
void hurtWorm(Worm *worm, int damage);

/**
    Increases the worms health by the healing factor.

    @param worm the worm gaining health
    @param healingFactor the amount of health the worm gains
*/
void healWorm(Worm *worm, int healingFactor);

/**
    Frees the given worm from memory.

    @param worm the worm being freed
*/
void freeWorm(Worm *worm);

void clearWorm(Worm *worm, Level *level);

#endif /* WORM_H */