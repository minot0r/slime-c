#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <stdint.h>
#include <SDL2/SDL.h>

#include "../other/linkedlist.h"

#include "key_manager.h"
#include "slime.h"
#include "ball.h"
#include "rect_collide.h"

typedef struct {
    bool is_running;
    float delta_time;
    SDL_Renderer* renderer;
    SDL_Window* window;
    key_manager_t key_manager;
    slime_t* slime_1;
    slime_t* slime_2;
    ball_t* ball;
    linked_list_t* rect_collides;
} game_state_t;

void game_init(game_state_t *game_state);
void game_process_events(game_state_t *game_state);
void game_update(game_state_t *game_state);
void game_check_collisions(game_state_t *game_state);
void game_debug(game_state_t *game_state);
void game_set_delta_time(game_state_t *game_state, float delta_time);
void game_render(game_state_t *game_state);
void game_destroy(game_state_t *game_state);

void register_rect_collide(game_state_t *game_state, rect_collide_t* rect_collide);

#endif