#ifndef SLIME_CONTROLLER_H
#define SLIME_CONTROLLER_H

#include <stdbool.h>
#include "../math/vector2.h"
#include "player_state.h"
#include "key_manager.h"
#include "rect_collide.h"

typedef struct slime_struct {
    int slime_id;
    vector2_t position;
    vector2_t center;
    int width;
    int height;
    bool on_ground;
    vector2_t velocity;
    vector2_t acceleration;
    player_state_t player_state;
    rect_collide_t* ground_collider;
    rect_collide_t* game_area_collider;
} slime_t;

slime_t* create_slime(int slime_id, rect_collide_t* ground_collider, rect_collide_t* game_area_collider);
void destroy_slime(slime_t* slime);
void update_slime(slime_t* slime, key_manager_t key_manager, float delta_time);
void render_slime(slime_t* slime, SDL_Renderer* renderer);

#endif