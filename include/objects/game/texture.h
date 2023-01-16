#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL_image.h>
#include "../engine/engine.h"

SDL_Texture* load_texture(engine_renderer_t* renderer, const char* path);

#endif 