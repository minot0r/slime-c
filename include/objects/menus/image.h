#ifndef IMAGE_H
#define IMAGE_H

#include <SDL2/SDL.h>
#include "../math/vector2.h"
#include "animation.h"

typedef struct {
    SDL_Texture* texture;
    SDL_Rect* rect;
    animation_t* animation;
} image_t;

image_t* create_image(SDL_Texture* texture, vector2_t position, vector2_t size);
void set_animation(image_t* image, animation_t* animation);
void image_update(image_t* image);
void destroy_image(image_t* image);

#endif