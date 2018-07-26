#include "text.h"
#define CHARS_IN_FONT 39
#define ALPHABET_LEN 26
#define PERIOD_IDX 36
#define PLUS_IDX 37
#define SPACE_IDX 38
#include <stdbool.h>


Font *loadFont(char *fileName, SDL_Surface *screen, Color background, Color textColor, int size) {
    Font *font = (Font *) malloc(sizeof(Font));
    font->screen = screen;
    font->size = size;
    font->textColor = textColor;
    font->backgroundColor = background;
    font->font = loadPPM(fileName);
    if(!font->font) {
        free(font);
        return NULL;
    }
    return font;
}
void freeFont(Font *font) {
    free(font->font);
    free(font);
}

void writeText(Font *font, char *text, int x, int y) {
    int letterHeight = font->font->height / CHARS_IN_FONT;
    int letterWidth = font->font->width;
    int letterIdx;
    for(int i = 0; text[i]; i++) {
        if('a' <= text[i] && text[i] <= 'z') {
            letterIdx = text[i] - 'a';
        }
        else if('A' <= text[i] && text[i] <= 'Z') {
            letterIdx = text[i] - 'A';
        }
        else if('0' <= text[i] && text[i] <= '9') {
            letterIdx = ALPHABET_LEN + text[i] - '0';
        } else if(text[i] == '.') {
            letterIdx = PERIOD_IDX;
        } else if(text[i] == '+') {
            letterIdx = PLUS_IDX;
        } else {
            letterIdx = SPACE_IDX;
        }
        
        drawSubImage(font->font, font->screen,
        x + i * letterWidth, y, 0, letterIdx * letterHeight,
        letterWidth, letterHeight, &font->backgroundColor,
        false, 0);
    }
}

void writeTextWithBackground(Font *font, char *text, int x, int y) {
    int old = font->backgroundColor.r;
    font->backgroundColor.r = 0;
    writeText(font, text, x, y);
    font->backgroundColor.r = old;
}

void clearText(Font *font, char *text, int x, int y) {
    
}

int stringDisplayWidth(Font *font, char *str) {
    return font->font->width * strlen(str);
}
int stringDisplayHeight(Font *font, char *str) {
    return (font->font->height / CHARS_IN_FONT);
}


