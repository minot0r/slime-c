#ifndef ANIMATION_H
#define ANIMATION_H

#include "../math/vector2.h"

typedef enum {
    EASE_IN,
    EASE_OUT,
    LINEAR
} animation_smoothing_t;

typedef enum {
    IDLE,
    RUNNING,
    FINISHED
} animation_state_t;

typedef struct {
    vector2_t position;
    vector2_t size;
    int angle;
    long tick_start;
    float progress;
    float duration;
    animation_state_t state;
    animation_smoothing_t smoothing;
} animation_t;

animation_t* create_animation(int starting_into_sec, vector2_t position, vector2_t size, float duration, animation_smoothing_t smoothing);
void update_animation(animation_t* animation);
void destroy_animation(animation_t* animation);


#endif