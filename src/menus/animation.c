#include "../../include/objects/menus/animation.h"

#include <math.h>
#include <SDL2/SDL.h>

animation_t* create_animation(int starting_into_sec, vector2_t position, vector2_t size, float duration, animation_smoothing_t smoothing) {
    animation_t* animation = malloc(sizeof(animation_t));
    animation->position = position;
    animation->size = size;
    animation->tick_start = SDL_GetTicks() + starting_into_sec * 1000;
    animation->progress = 0.0f;
    animation->duration = duration;
    animation->state = IDLE;
    animation->smoothing = smoothing;
    
    return animation;
}

void update_animation(animation_t* animation) {
    switch (animation->state) {
        case IDLE:
            if (SDL_GetTicks() >= animation->tick_start) {
                animation->state = RUNNING;
            }
            break;
        case RUNNING:
            animation->progress = ((float) (SDL_GetTicks() - animation->tick_start)) / (animation->duration);
            if (animation->progress >= 1) {
                animation->progress = 1;
                animation->state = FINISHED;
            } else {
                switch (animation->smoothing) {
                    case EASE_IN:
                        animation->progress = pow(animation->progress, 2);
                        break;
                    case EASE_OUT:
                        animation->progress = 1 - pow(1 - animation->progress, 2);
                        break;
                    case LINEAR:
                    default:
                        break;
                }
            }
            break;
        case FINISHED:
        default:
            break;
    }
}

void destroy_animation(animation_t* animation) {
    free(animation);
}