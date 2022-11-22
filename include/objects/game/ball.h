#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include "../math/vector2.h"

#include <SDL2/SDL.h>

typedef struct {
    vector2_t position;
    vector2_t center;
    int width;
    int height;
    bool on_ground;
    vector2_t velocity;
    vector2_t acceleration;
} ball_t;

ball_t* create_ball();
void destroy_ball(ball_t* ball);
void update_ball(ball_t* ball, float delta_time);
void render_ball(ball_t* ball, SDL_Renderer* renderer);
void apply_force_to_ball(ball_t* ball, vector2_t* force);
vector2_t* get_force_to_apply_ball_sphere(ball_t* ball, vector2_t center_sphere, float radius_sphere);
#endif