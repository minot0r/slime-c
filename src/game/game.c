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

    // get random slime texture
    int slime_texture_index = rand() % 3;
    const char *slime_texture_1_asset = slime_texture_assets[slime_texture_index];
    slime_texture_index = rand() % 3;
    const char *slime_texture_2_asset = slime_texture_assets[slime_texture_index];

    SDL_Texture* texture_1 = load_texture(renderer, slime_texture_1_asset);
    SDL_Texture* texture_2 = load_texture(renderer, slime_texture_2_asset);


    game->slime_1 = create_slime(0, texture_1, get_rect_collide(game, "GROUND"), get_rect_collide(game, "LEFT_SIDE"));
    game->slime_2 = create_slime(1, texture_2, get_rect_collide(game, "GROUND"), get_rect_collide(game, "RIGHT_SIDE"));
    game->ball = create_ball(
        get_rect_collide(game, "GROUND"),
        get_rect_collide(game, "WORLD"),
        get_rect_collide(game, "NET")
    );

    ball_set_position(game->ball, (vector2_t) { game->slime_1->center.x - game->ball->width / 2, 0 });
    
    game->started = false;
}

void game_update(game_state_t *game_state, float delta_time, key_manager_t key_manager) {
    update_slime(game_state->slime_1, key_manager, delta_time);
    update_ball(game_state->ball, delta_time);
    update_slime(game_state->slime_2, key_manager, delta_time);
}

void game_check_collisions(game_state_t *game_state) {
    if(sphere_colliding(
        game_state->slime_1->center, game_state->slime_1->width / 2,
        game_state->ball->center, game_state->ball->width / 2)
    ) {
        vector2_t* force = get_force_to_apply_ball_sphere(game_state->ball, game_state->slime_1->center, game_state->slime_1->width / 2);
        /* force->x += game_state->slime_2->velocity.x / 10;
        force->y += game_state->slime_2->velocity.y / 10;  */
        if(force->y >= 0) return;
        printf("force: %f, %f\n", force->x, force->y);
        apply_force_to_ball(game_state->ball, force);
    }

    if(sphere_colliding(
        game_state->slime_2->center, game_state->slime_2->width / 2,
        game_state->ball->center, game_state->ball->width / 2)
    ) {
        vector2_t* force = get_force_to_apply_ball_sphere(game_state->ball, game_state->slime_2->center, game_state->slime_2->width / 2);
        /* force->x += game_state->slime_2->velocity.x / 10;
        force->y += game_state->slime_2->velocity.y / 10; */
        if(force->y >= 0) return;
        apply_force_to_ball(game_state->ball, force);
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
    render_text(renderer, "Hello World!", 10, 10, 32, 255, 255, 255, 255);
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