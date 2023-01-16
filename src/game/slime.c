#include "../../include/objects/game/slime.h"
#include "../../include/objects/game/player_state.h"

#include <stdlib.h>

slime_t* create_slime(
    int slime_id,
    SDL_Texture* texture,
    rect_collide_t* ground_collider,
    rect_collide_t* game_area_collider
) {
    slime_t* slime = malloc(sizeof(slime_t));
    slime->position = (vector2_t) { game_area_collider->position.x + (game_area_collider->width - 80) / 2, 0 };
    slime->center = (vector2_t) { slime->position.x + 80 / 2, slime->position.y + 40 };
    slime->width = 80;
    slime->height = 40;
    slime->on_ground = false;
    slime->velocity = (vector2_t) { 0, 0 };
    slime->acceleration = (vector2_t) { 0, 0 };
    slime->slime_id = slime_id;
    slime->ground_collider = ground_collider;
    slime->game_area_collider = game_area_collider;
    slime->texture = texture;

    create_player_state(&slime->player_state, slime_id == 0);

    return slime;
}

void destroy_slime(slime_t* slime) {
    free(slime);
}

void update_slime(slime_t* slime, key_manager_t key_manager, float delta_time) {
    if(slime->slime_id == 0) {
        if(key_manager.key_d_down) {
            slime->velocity.x = 300;
        } else if(key_manager.key_q_down) {
            slime->velocity.x = -300;
        } else {
            slime->velocity.x = 0;
        }
        if(key_manager.key_z_down && slime->on_ground) {
            slime->velocity.y = -850;
        }
    } else if(slime->slime_id == 1) {
        if(key_manager.key_right_down) {
            slime->velocity.x = 300;
        } else if(key_manager.key_left_down) {
            slime->velocity.x = -300;
        } else {
            slime->velocity.x = 0;
        }
        if(key_manager.key_up_down && slime->on_ground) {
            slime->velocity.y = -850;
        }
    }

    slime->velocity.y += 3000 * delta_time;

    slime->position.x += slime->velocity.x * delta_time;
    slime->position.y += slime->velocity.y * delta_time;

    slime->center.x = slime->position.x + slime->width / 2;
    slime->center.y = slime->position.y + slime->height;

    // check collision with ground
    collide_dir_e collide_dir = is_colliding_y(slime->ground_collider, slime->position, slime->width, slime->height);
    if(collide_dir != NONE) {
        slime->center.y = slime->ground_collider->position.y;
        slime->position.y = slime->center.y - slime->height;
        slime->velocity.y = 0;
        slime->on_ground = true;
    } else {
        slime->on_ground = false;
    }

    // check collision with game area
    collide_dir = is_colliding_x(slime->game_area_collider, slime->position, slime->width, slime->height);
    if(collide_dir == LEFT) {
        slime->center.x = slime->game_area_collider->position.x + slime->width / 2;
        slime->position.x = slime->center.x - slime->width / 2;
        slime->velocity.x = 0;
    } else if(collide_dir == RIGHT) {
        slime->center.x = slime->game_area_collider->position.x + slime->game_area_collider->width - slime->width / 2;
        slime->position.x = slime->center.x - slime->width / 2;
        slime->velocity.x = 0;
    }
}

void render_slime(slime_t* slime, engine_renderer_t* renderer) {
    /* SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    int radius = slime->width / 2;

    int center_x = slime->center.x;
    int center_y = slime->center.y;

    int offset_x, offset_y, d;

    offset_x = 0;
    offset_y = radius;
    d = radius - 1;

    while (offset_y >= offset_x) {
        SDL_RenderDrawLine(renderer, center_x - offset_x, center_y - offset_y, center_x + offset_x, center_y - offset_y);
        SDL_RenderDrawLine(renderer, center_x - offset_y, center_y - offset_x, center_x + offset_y, center_y - offset_x);

        if (d >= 2*offset_x) {
            d -= 2*offset_x + 1;
            offset_x +=1;
        }
        else if (d < 2 * (radius - offset_y)) {
            d += 2 * offset_y - 1;
            offset_y -= 1;
        }
        else {
            d += 2 * (offset_y - offset_x - 1);
            offset_y -= 1;
            offset_x += 1;
        }
    } */
    SDL_RenderCopyEx(
        renderer->r_w,
        slime->texture,
        NULL,
        &(SDL_Rect) { slime->position.x * renderer->scale,
        slime->position.y * renderer->scale,
        slime->width * renderer->scale,
        slime->height * renderer->scale },
        0, NULL, slime->slime_id == 0 ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
    SDL_SetRenderDrawColor(renderer->r_w, 255, 255, 0, 255);
/*     // draw rect for position
    SDL_Rect rect = { slime->position.x - 1, slime->position.y - 1, 2, 2 };
    // draw rect for center
    SDL_Rect rect2 = { slime->center.x - 1, slime->center.y - 1, 2, 2 };
    SDL_RenderFillRect(renderer->, &rect);
    SDL_RenderFillRect(renderer, &rect2); */
}