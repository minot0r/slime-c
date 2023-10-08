#ifndef BALL_H
#define BALL_H

#include <stdbool.h>
#include "../engine/engine.h"
#include "../math/vector2.h"
#include "slime.h"
#include "rect_collide.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define BALL_RADIUS 12

typedef struct {
    vector2_t position;
    vector2_t center;
    int width;
    int height;
    bool on_ground;
    vector2_t velocity;
    vector2_t acceleration;
    rect_collide_t* ground_collider;
    rect_collide_t* world_collider;
    rect_collide_t* net_collider;
    SDL_Texture* texture;
    Mix_Chunk* bounce_sound;
} ball_t;

ball_t* create_ball(SDL_Texture* texture, rect_collide_t* ground_collider, rect_collide_t* world_collider, rect_collide_t* net_collider);
void destroy_ball(ball_t* ball);
void update_ball(ball_t* ball, float delta_time, slime_t* slime1, slime_t* slime2);
void render_ball(ball_t* ball, engine_renderer_t* renderer);
void ball_set_position(ball_t* ball, vector2_t position);
void check_collision(ball_t* ball, slime_t* slime);
#endif