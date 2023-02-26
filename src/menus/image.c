#include "../../include/objects/menus/image.h"

image_t *create_image(SDL_Texture *texture, vector2_t position, vector2_t size) {
    image_t *image = malloc(sizeof(image_t));
    image->texture = texture;
    SDL_Rect *rect = malloc(sizeof(SDL_Rect));
    rect->x = position.x;
    rect->y = position.y;
    rect->w = size.x;
    rect->h = size.y;
    image->rect = rect;
    image->animation = NULL;
    return image;
}

void set_animation(image_t *image, animation_t *animation) {
    image->animation = animation;
}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

void image_update(image_t *image) {
    if (image->animation != NULL) {
        update_animation(image->animation);
        float progress = image->animation->progress;
        image->rect->x = lerp(image->rect->x, image->animation->position.x, progress);
        image->rect->y = lerp(image->rect->y, image->animation->position.y, progress);
        image->rect->w = lerp(image->rect->w, image->animation->size.x, progress);
        image->rect->h = lerp(image->rect->h, image->animation->size.y, progress);
    }
}

void destroy_image(image_t *image) {
    SDL_DestroyTexture(image->texture);
    free(image->rect);
    if(image->animation != NULL)
        destroy_animation(image->animation);
    free(image);
}