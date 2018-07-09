#include "anim.h"

/**
    Loads an animation from the given file. Width and height pertain
    to the size of each frame in the animation sheet. Fps controls
    the number of frames played per second.
*/
Anim *loadAnim(char *fileName, int width, int height, float fps, Color *background);

/**
    Frees an animation from memory.
*/
void freeAnim(Anim *anim);