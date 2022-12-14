#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>

void render_text(SDL_Renderer* renderer, const char *text, int x, int y, int size, int r, int g, int b, int a);

#endif