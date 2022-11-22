#include "../../include/objects/game/game.h"
#include "../../include/objects/math/collider.h"
#include "../../include/objects/game/rect_collide.h"

#include <stdlib.h>

void game_init(game_state_t *game) {
    game->is_running = false;
    game->rect_collides = linked_list_init();
    key_manager_init(&game->key_manager);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    game->window = SDL_CreateWindow(
        "SDL2 OpenGL", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    if(game->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    game->renderer = SDL_CreateRenderer(game->window, -1, SDL_RENDERER_SOFTWARE);

    if(game->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    game->slime_1 = create_slime(0);
    game->slime_2 = create_slime(1);
    game->ball = create_ball();


    register_rect_collide(game, create_rect_collide(
        "GROUND",
        0,
        400,
        640,
        80,
        OUTSIDE,
        (color_t) { 0, 0, 255, 255 },
        false)
    );

    game->is_running = true;
    game->delta_time = 0;
}

void game_process_events(game_state_t *game_state) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            game_state->is_running = false;
            break;
        case SDL_KEYDOWN:
            key_manager_process_events(&game_state->key_manager, event.key.keysym.sym, true);
            break;
        case SDL_KEYUP:
            key_manager_process_events(&game_state->key_manager, event.key.keysym.sym, false);
            break;
        default:
            break;
    }
}

void game_update(game_state_t *game_state) {
    update_slime(game_state->slime_1, game_state->key_manager, game_state->delta_time);
    update_ball(game_state->ball, game_state->delta_time);
    update_slime(game_state->slime_2, game_state->key_manager, game_state->delta_time);
}

void game_check_collisions(game_state_t *game_state) {
    if(sphere_colliding(
        game_state->slime_1->center, game_state->slime_1->width / 2,
        game_state->ball->center, game_state->ball->width / 2)
    ) {
        vector2_t* force = get_force_to_apply_ball_sphere(game_state->ball, game_state->slime_1->center, game_state->slime_1->width / 2);
        apply_force_to_ball(game_state->ball, force);
    }

    if(sphere_colliding(
        game_state->slime_2->center, game_state->slime_2->width / 2,
        game_state->ball->center, game_state->ball->width / 2)
    ) {
        vector2_t* force = get_force_to_apply_ball_sphere(game_state->ball, game_state->slime_2->center, game_state->slime_2->width / 2);
        apply_force_to_ball(game_state->ball, force);
    }
}

void game_debug(game_state_t *game_state) {
    if(game_state->key_manager.key_d_down) {
        printf("Key D is down\n");
    }
    if(game_state->key_manager.key_z_down) {
        printf("Key Z is down\n");
    }
    if(game_state->key_manager.key_q_down) {
        printf("Key Q is down\n");
    }
    if(game_state->key_manager.key_up_down) {
        printf("Key UP is down\n");
    }
    if(game_state->key_manager.key_left_down) {
        printf("Key LEFT is down\n");
    }
    if(game_state->key_manager.key_right_down) {
        printf("Key RIGHT is down\n");
    }
    if(!game_state->slime_1->on_ground) {
        printf("%f\n", game_state->slime_1->velocity.y);
    }
}

void game_set_delta_time(game_state_t *game_state, float delta_time) {
    game_state->delta_time = delta_time;
}

void game_render(game_state_t *game_state) {
    SDL_SetRenderDrawColor(game_state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(game_state->renderer);
    render_slime(game_state->slime_1, game_state->renderer);
    render_slime(game_state->slime_2, game_state->renderer);
    render_ball(game_state->ball, game_state->renderer);
    for(int i = 0; i < game_state->rect_collides->size; i++) {
        rect_collide_t* rect_collide = (rect_collide_t*) linked_list_get(game_state->rect_collides, i);
        render_rect_collide(rect_collide, game_state->renderer);
    }
    SDL_RenderPresent(game_state->renderer);
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
    SDL_DestroyRenderer(game_state->renderer);
    SDL_DestroyWindow(game_state->window);
    SDL_Quit();
}

void register_rect_collide(game_state_t* game_state, rect_collide_t* rect_collide) {
    linked_list_add(game_state->rect_collides, rect_collide);
}