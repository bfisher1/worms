/**
    @file anim.h
    @author Ben Fisher

    Defines enumerations for recalling animations, the structure
    of an animation, and functions for loading and playing
    animations.
*/
#ifndef ANIM_H
#define ANIM_H

#include <SDL/SDL.h>
#include "graphics.h"
#include <stdbool.h>
#include <time.h>

typedef enum {
    wormMove,
    wormYawn,
    wormStill
} AnimName;

/**
    Structure for an animation. It contains a pointer to the screen
    it will be drawn on to simplify drawing functions. Also contains
    the number of frames, the images that make up an animation, and
    the frames per second it is played at. Contains flags for if the
    animation is flipped horizontally or repeats endlessly.
*/
typedef struct {
    SDL_Surface *screen;
    int frames;
    Image *pics;
    float fps;
    bool flippedHoriz;
    bool repeat;
    clock_t lastPlayed;
} Anim;

/**
    Loads the animations needed for the game. Allocates an array of
    animations that it saves the length of. Animations are stored at
    indexes pertaining to their enumeration.

    @param len pointer to an integer that will be overwritten with
           the animation array's length

    @return pointer to an array of animations
*/
Anim *loadAnims(int *len);

/**
    Frees an allocated array of anims of length len.

    @param anims the anims being freed
    @param len the number of anims being freed
*/
void freeAnims(Anim *anims, int len);

/**
    Plays an animation, returning true if it has finished.

    @param anim the anim being played

    @return true if the animation has finished, false if not
*/
bool playAnim(Anim *anim);

#endif /* ANIM_H */