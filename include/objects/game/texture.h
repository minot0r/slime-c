#ifndef TEXTURE_H
#define TEXTURE_H

#include <SDL2/SDL_image.h>

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path);

#endif 