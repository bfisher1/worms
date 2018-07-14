#include "anim.h"
#include <stdlib.h>
/**
    Loads an animation from the given file. Width and height pertain
    to the size of each frame in the animation sheet. Fps controls
    the number of frames played per second.

    @param fileName the name of the animation sprite sheet
    @param frames the number of frames in the animation
    @param width the width of the anim's frames
    @param height the height of the anim's frame
    @param fps the frames played each second by the anim
    @param screen the screen the anim is played on
    @param background the background of the spritesheet, this color
           is ignored when drawing the animation

    @return the created anim
*/
Anim *loadAnim(char *fileName, int frames, int width, int height, float fps, SDL_Surface *screen, Color *background) {
    Anim *anim = (Anim *) malloc(sizeof(Anim));
    anim->screen = screen;
    anim->frames = frames;
    anim->spriteSheet = loadPPM(fileName);
    if(anim->spriteSheet == NULL) {
        free(anim);
        return NULL;
    }
    anim->fps = fps;
    anim->flippedHoriz = false;
    anim->repeat = true;
    anim->lastPlayed = clock();
    anim->background = *background;
    anim->currentFrame = 0;
    anim->width = anim->spriteSheet->width;
    anim->height = anim->spriteSheet->height / anim->frames;
    return anim;
}

/**
    Frees an animation from memory.

    @param anim the anim being freed
*/
void freeAnim(Anim *anim) {
    freeImage(anim->spriteSheet);
    free(anim);
}


Anim **loadAnims(SDL_Surface *screen, int *len) {
    *len = ANIMS_NUM;
    Color blue = {0, 0, 255};
    Anim **anims = (Anim **) malloc(sizeof(Anim *) * *len);
    anims[wormMove] = loadAnim("anims/worm move.ppm", 10, 40, 40, 10, screen, &blue);
    anims[wormYawn] = loadAnim("anims/worm yawn.ppm", 10, 40, 40, 1, screen, &blue);
    anims[wormStill] = loadAnim("anims/worm still.ppm", 1, 40, 40, 1, screen, &blue);
    for(int i = 0; i < *len; i++){
        //if one of the anims wasn't read properly, then return NULL
        if(!anims[i]) {
            for( i = 0; i < *len; i++){
                if(anims[i]){
                    freeAnim(anims[i]);
                }
            }
            return NULL;
        }
    }
    return anims;
}

void freeAnims(Anim **anims, int len) {
    for(int i = 0; i < len; i++){
        freeAnim(anims[i]);
    }
    free(anims);
}

bool playAnim(Anim *anim, int x, int y, float angle) {
    if( (float) (clock() - anim->lastPlayed) / CLOCKS_PER_SEC >= (1.0 / anim->fps) ){
        drawSubImage(anim->spriteSheet, anim->screen,
        x, y, 0, anim->currentFrame * anim->height,
        anim->width, anim->height, &anim->background,
        anim->flippedHoriz, angle);

        anim->currentFrame++;
        if(anim->currentFrame >= anim->frames) {
            anim->currentFrame = 0;
            return true;
        }
    }
    return false;
}