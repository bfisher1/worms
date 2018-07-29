#include "anim.h"
#include <stdlib.h>
#define ANIMS_NUM 20

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
    anim->background = *background;
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
    Color red = {255, 0, 0};
    Anim **anims = (Anim **) malloc(sizeof(Anim *) * *len);
    anims[wormMove] = loadAnim("anims/worm move.ppm", 10, 40, 40, 18, screen, &blue);
    anims[wormYawn] = loadAnim("anims/worm yawn.ppm", 10, 40, 40, 5, screen, &blue);
    anims[wormStill] = loadAnim("anims/worm still.ppm", 1, 40, 40, 1, screen, &blue);
    anims[dynamiteAnim] = loadAnim("anims/dynamite.ppm", 5, 40, 40, 25, screen, &blue);
    anims[explosion] = loadAnim("anims/explosion2.ppm", 7, 76, 78, 20, screen, &red);
    anims[healthCrate] = loadAnim("anims/healthCrate.ppm", 1, 20, 20, 1, screen, &blue);
    anims[weaponCrate] = loadAnim("anims/weaponCrate.ppm", 1, 20, 20, 1, screen, &red);
    anims[blowTorchStill] = loadAnim("anims/blow torch.ppm", 1, 80, 40, 1, screen, &blue);
    anims[blowTorchFire] = loadAnim("anims/blow torch fire.ppm", 5, 80, 40, 30, screen, &blue);
    anims[mineOffAnim] = loadAnim("anims/mine off.ppm", 1, 26, 15, 1, screen, &blue);
    anims[mineBlinkAnim] = loadAnim("anims/mine blink.ppm", 2, 26, 15, 4, screen, &blue);
    anims[crossHair] = loadAnim("anims/cross hair.ppm", 1, 19, 19, 1, screen, &blue);
    anims[bazookaAnim] = loadAnim("anims/bazooka.ppm", 1, 80, 40, 1, screen, &blue);
    anims[missileAnim] = loadAnim("anims/bazooka shell.ppm", 1, 14, 12, 1, screen, &blue);
    anims[pistolAnim] = loadAnim("anims/pistol.ppm", 1, 30, 30, 1, screen, &blue);
    anims[bulletAnim] = loadAnim("anims/bullet.ppm", 1, 9, 9, 1, screen, &blue);
    anims[smokeAnim] = loadAnim("anims/smoke.ppm", 10, 40, 40, 12, screen, &blue);
    anims[invAnim] = loadAnim("anims/inventory.ppm", 1, 188, 77, 1, screen, &blue);
    anims[selectedCell] = loadAnim("anims/selected cell.ppm", 1, 34, 34, 1, screen, &blue);
    anims[bigExplosion] = loadAnim("anims/big explosion.ppm", 7, 152, 156, 15, screen, &red);
    //dynamite 25 fps
    //explosion 20 fps
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

bool playAnim(Anim *anim, int x, int y, float angle, int *frame, clock_t *lastPlayed, bool flippedHoriz) {
    drawSubImage(anim->spriteSheet, anim->screen,
    x, y, 0, *frame * anim->height,
    anim->width, anim->height, &anim->background,
    flippedHoriz, angle);

    if( (float) (clock() - *lastPlayed) / CLOCKS_PER_SEC >= (1.0 / anim->fps) ){
       *lastPlayed = clock();
       *frame += 1;
        if(*frame >= anim->frames) {
            *frame = 0;
            return true;
        }
    }
    return false;
}