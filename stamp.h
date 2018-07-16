#ifndef STAMP_H
#define STAMP_H
#include "anim.h"
#include "stdbool.h"
#include <time.h>
#include "level.h"

typedef struct {
	Anim *anim;
	float duration;
	bool repeat;
	int frame;
	bool hasFinished;
	clock_t start;
	int x;
	int y;
} Stamp;

Stamp *createStamp(Anim *anim, float duration, bool repeat, int x, int y);
void drawStamp(Stamp *stamp, int x, int y);
void freeStamp(Stamp *stamp);
void clearStamp(Stamp *stamp, Level *level, int x, int y);
bool hasStampFinished(Stamp *stamp);
#endif /* STAMP_H */
