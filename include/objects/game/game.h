#ifndef GAME_H
#define GAME_H

#include <stdbool.h>
#include <SDL2/SDL.h>

#include "../other/linkedlist.h"

#include "../engine/engine.h"
#include "../menus/item.h"
#include "slime.h"
#include "ball.h"
#include "rect_collide.h"

#define POINTS_TO_WIN 6

typedef void (*callback)(void);

typedef struct {
    bool started;
    slime_t* slime_1;
    slime_t* slime_2;
    ball_t* ball;
    linked_list_t* rect_collides;
    int points_1;
    int points_2;
    long unfreeze_time;
    callback on_done;
} game_state_t;

void game_init(game_state_t *game_state, engine_renderer_t* renderer);

void game_update(game_state_t *game_state, float delta_time, key_manager_t key_manager);
void game_render(game_state_t *game_state, engine_renderer_t* renderer);
void game_destroy(game_state_t *game_state);
void register_rect_collide(game_state_t *game_state, rect_collide_t* rect_collide);
rect_collide_t* get_rect_collide(game_state_t *game_state, char* name);
void game_on_done(game_state_t* game_state, callback cb);
void game_reset(game_state_t *game_state);
void set_freeze(game_state_t *game_state, long seconds);
bool is_freezed(game_state_t *game_state);

#endif