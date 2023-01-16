#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "../other/linkedlist.h"

#include "../engine/engine.h"
#include "slime.h"
#include "ball.h"
#include "rect_collide.h"

typedef struct {
    bool started;
    slime_t* slime_1;
    slime_t* slime_2;
    ball_t* ball;
    linked_list_t* rect_collides;
} game_state_t;

void game_init(game_state_t *game_state, engine_renderer_t* renderer);
void game_update(game_state_t *game_state, float delta_time, key_manager_t key_manager);
void game_check_collisions(game_state_t *game_state);
void game_render(game_state_t *game_state, engine_renderer_t* renderer);
void game_destroy(game_state_t *game_state);

void register_rect_collide(game_state_t *game_state, rect_collide_t* rect_collide);
rect_collide_t* get_rect_collide(game_state_t *game_state, char* name);

#endif