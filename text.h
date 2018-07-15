#ifndef TEXT_H
#define TEXT_H
#include <SDL/SDL.h>
#include "graphics.h"
#include <stdlib.h>
typedef struct {
    Image *font;
    int size;
    Color backgroundColor;
    Color textColor;
    SDL_Surface *screen;
} Font;

Font *loadFont(char *fileName, SDL_Surface *screen, Color background, Color textColor, int size);
void freeFont(Font *font);
void writeText(Font *font, char *text, int x, int y);
void clearText(Font *font, char *text, int x, int y);

#endif /* TEXT_H */
