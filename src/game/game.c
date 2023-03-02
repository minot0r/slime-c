#include "../../include/objects/game/game.h"
#include "../../include/objects/math/collider.h"
#include "../../include/objects/game/rect_collide.h"
#include "../../include/objects/game/texture.h"
#include "../../include/objects/menus/text_renderer.h"


const char slime_texture_assets[3][32] = {
    "assets/slime/slime_dino.png",
    "assets/slime/slime_jelly.png",
    "assets/slime/slime_france.png"
};

int pick_random_texture() {
    return rand() % 3;
}

void game_init(game_state_t *game, engine_renderer_t* renderer) {
    game->rect_collides = linked_list_init();

    register_rect_collide(game, create_rect_collide(
        "GROUND",
        0,
        ENGINE_RESOLUTION_Y - 80,
        ENGINE_RESOLUTION_X,
        80,
        OUTSIDE,
        (color_t) { 0, 0, 255, 255 },
        false)
    );

    register_rect_collide(game, create_rect_collide(
        "GROUND_LEFT",
        0,
        ENGINE_RESOLUTION_Y - 80,
        ENGINE_RESOLUTION_X / 2 - 5,
        80,
        OUTSIDE,
        (color_t) { 0, 0, 0, 0 },
        true)
    );

    register_rect_collide(game, create_rect_collide(
        "GROUND_RIGHT",
        ENGINE_RESOLUTION_X / 2 + 5,
        ENGINE_RESOLUTION_Y - 80,
        ENGINE_RESOLUTION_X,
        80,
        OUTSIDE,
        (color_t) { 0, 0, 0, 0 },
        true)
    );

    register_rect_collide(game, create_rect_collide(
        "WORLD",
        0,
        0,
        ENGINE_RESOLUTION_X,
        ENGINE_RESOLUTION_X,
        INSIDE,
        (color_t) { 255, 0, 0, 255 },
        true)
    );

    register_rect_collide(game, create_rect_collide(
        "LEFT_SIDE",
        0,
        0,
        ENGINE_RESOLUTION_X / 2 - 5,
        ENGINE_RESOLUTION_Y,
        INSIDE,
        (color_t) { 255, 0, 0, 255 },
        true)
    );

    register_rect_collide(game, create_rect_collide(
        "NET",
        ENGINE_RESOLUTION_X / 2 - 5,
        ENGINE_RESOLUTION_Y - 115,
        10,
        115,
        OUTSIDE,
        (color_t) { 0, 0, 255, 255 },
        false)
    );

    register_rect_collide(game, create_rect_collide(
        "RIGHT_SIDE",
        ENGINE_RESOLUTION_X / 2 + 5,
        0,
        ENGINE_RESOLUTION_X / 2 - 5,
        ENGINE_RESOLUTION_Y,
        INSIDE,
        (color_t) { 255, 0, 0, 255 },
        true)
    );

    SDL_Texture* texture_1 = load_texture(renderer, slime_texture_assets[pick_random_texture()]);
    SDL_Texture* texture_2 = load_texture(renderer, slime_texture_assets[pick_random_texture()]);

    game->slime_1 = create_slime(0, texture_1, get_rect_collide(game, "GROUND_LEFT"), get_rect_collide(game, "LEFT_SIDE"));
    game->slime_2 = create_slime(1, texture_2, get_rect_collide(game, "GROUND_RIGHT"), get_rect_collide(game, "RIGHT_SIDE"));
    game->ball = create_ball(
        get_rect_collide(game, "GROUND"),
        get_rect_collide(game, "WORLD"),
        get_rect_collide(game, "NET")
    );

    ball_set_position(game->ball, (vector2_t) { game->slime_1->center.x - game->ball->width / 2, 0 });
    
    game->started = false;
}

void game_update(game_state_t *game_state, float delta_time, key_manager_t key_manager) {
    int sub_steps = 3;
    float sub_dt = delta_time / sub_steps;

    for(int i = 0; i < sub_steps; i++) {
        update_slime(game_state->slime_1, key_manager, sub_dt);
        update_ball(game_state->ball, sub_dt, game_state->slime_1, game_state->slime_2);
        update_slime(game_state->slime_2, key_manager, sub_dt);
    
        if(is_colliding_y(get_rect_collide(game_state, "GROUND_LEFT"), game_state->ball->position, game_state->ball->width, game_state->ball->height)) {
            game_state->points_2++;
            set_freeze(game_state, 2);
            reset_slime(game_state->slime_1);
            reset_slime(game_state->slime_2);
            ball_set_position(game_state->ball, (vector2_t) { game_state->slime_2->center.x - game_state->ball->width / 2, 0 });
        } else if(is_colliding_y(get_rect_collide(game_state, "GROUND_RIGHT"), game_state->ball->position, game_state->ball->width, game_state->ball->height)) {
            game_state->points_1++;
            set_freeze(game_state, 2);
            reset_slime(game_state->slime_1);
            reset_slime(game_state->slime_2);
            ball_set_position(game_state->ball, (vector2_t) { game_state->slime_1->center.x - game_state->ball->width / 2, 0 });
        }

        if(game_state->points_1 == POINTS_TO_WIN || game_state->points_2 == POINTS_TO_WIN) {
            set_freeze(game_state, 2);
            game_reset(game_state);
        }
    }
}

void game_render(game_state_t* game_state, engine_renderer_t* renderer) {
    SDL_SetRenderDrawColor(renderer->r_w, 0, 0, 0, 255);
    SDL_RenderClear(renderer->r_w);
    render_slime(game_state->slime_1, renderer);
    render_slime(game_state->slime_2, renderer);
    render_ball(game_state->ball, renderer);
    for(int i = 0; i < game_state->rect_collides->size; i++) {
        rect_collide_t* rect_collide = (rect_collide_t*) linked_list_get(game_state->rect_collides, i);
        render_rect_collide(rect_collide, renderer);
    }
    char* score = (char*) malloc(6 * sizeof(char));
    sprintf(score, "%d - %d", game_state->points_1, game_state->points_2);
    render_text(renderer, score, ENGINE_RESOLUTION_X / 2 - 10, 10, 10, 255, 255, 255, 255);
    SDL_RenderPresent(renderer->r_w);
}

void game_destroy(game_state_t *game_state) {
    destroy_slime(game_state->slime_1);
    destroy_slime(game_state->slime_2);
    destroy_ball(game_state->ball);
    for(int i = 0; i < game_state->rect_collides->size; i++) {
        rect_collide_t* rect_collide = (rect_collide_t*) linked_list_get(game_state->rect_collides, i);
        destroy_rect_collide(rect_collide);
    }
    linked_list_destroy(game_state->rect_collides);
}

void register_rect_collide(game_state_t* game_state, rect_collide_t* rect_collide) {
    linked_list_add(game_state->rect_collides, rect_collide);
}

rect_collide_t* get_rect_collide(game_state_t *game_state, char* name) {
    for(int i = 0; i < game_state->rect_collides->size; i++) {
        rect_collide_t* rect_collide = (rect_collide_t*) linked_list_get(game_state->rect_collides, i);
        if(strcmp(rect_collide->name, name) == 0) {
            return rect_collide;
        }
    }
    return NULL;
}

void game_on_done(game_state_t* game_state, callback cb) {
    game_state->on_done = cb;
}

void game_reset(game_state_t *game_state) {
    game_state->points_1 = 0;
    game_state->points_2 = 0;
    reset_slime(game_state->slime_1);
    reset_slime(game_state->slime_2);
    ball_set_position(game_state->ball, (vector2_t) { game_state->slime_1->center.x - game_state->ball->width / 2, 0 });

    if(game_state->on_done != NULL) {
        game_state->on_done();
    }
}

void set_freeze(game_state_t* game_state, long seconds) {
    game_state->unfreeze_time = SDL_GetTicks() + seconds * 1000;
}

bool is_freezed(game_state_t* game_state) {
    return SDL_GetTicks() < game_state->unfreeze_time;
}