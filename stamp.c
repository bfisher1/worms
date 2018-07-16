#include "stamp.h"
#include <stdlib.h>

Stamp *createStamp(Anim *anim, float duration, bool repeat, int x, int y) {
    Stamp *stamp = (Stamp *) malloc(sizeof(Stamp));
    stamp->anim = anim;
    stamp->frame = 0;
    stamp->duration = duration;
    stamp->repeat = repeat;
    stamp->hasFinished = false;
    stamp->start = clock();
    stamp->x = x;
    stamp->y = y;
    return stamp;
}
void drawStamp(Stamp *stamp, int x, int y) {
    if(playAnim(stamp->anim, x, y, 0, &stamp->frame)){
        stamp->hasFinished = true;
    }
}
void freeStamp(Stamp *stamp) {
    free(stamp);
}

void clearStamp(Stamp *stamp, Level *level, int x, int y) {
    drawLevel(level, (x - stamp->anim->width) / 8,
     (y - stamp->anim->height) / 8,
    (x + stamp->anim->width) ,
    (y + stamp->anim->height) );
}

bool hasStampFinished(Stamp *stamp) {
    if(stamp->repeat){
        if( ((float) (clock() - stamp->start)) / CLOCKS_PER_SEC >= stamp->duration ) {
            return true;
        }
        return false;
    }
    return stamp->hasFinished;
}