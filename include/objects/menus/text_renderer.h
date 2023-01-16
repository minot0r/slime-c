#ifndef TEXT_RENDERER_H
#define TEXT_RENDERER_H

#include <SDL2/SDL.h>
#include "../engine/engine.h"

void render_text(engine_renderer_t* renderer, const char *text, int x, int y, int size, int r, int g, int b, int a);

#endif