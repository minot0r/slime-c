#include <stdio.h>

#include "../include/objects/engine/engine.h"
#include "../include/objects/game/texture.h"
#include "../include/objects/game/game.h"
#include "../include/objects/menus/menu.h"
#include "../include/objects/menus/item.h"
#include "../include/objects/menus/text_renderer.h"

#define SDL_MAIN_HANDLED
#define FPS 60.0f
#define SPEED 0.9f

engine_t engine;
game_state_t game;
menu_t *menu;

void start_game() {
    game.started = true;
}

void leave_game() {
    engine.is_running = false;
}

bool is_in_menu() {
    return !game.started;
}

bool pause_trigger_valid() {
    return !is_in_menu() && engine.key_manager.key_space_down;
}

bool resume_trigger_valid() {
    return !is_in_menu() && !engine.key_manager.key_space_down;
}

void goto_menu() {
    game.started = false;
}

int main() {
    engine_init(&engine);

    menu = menu_create(NULL, (SDL_Color){0, 0, 0, 255});
    item_t *item = item_create("SLIME", false, (vector2_t){0, 0}, 32);
    item_t *item1 = item_create("Commencer une partie", true, (vector2_t){0, 64}, 24);
    item_t *item2 = item_create("Quitter", true, (vector2_t){0, 96}, 24);
    image_t *image = create_image(load_texture(engine.renderer, "assets/slime/slime_dino.png"), (vector2_t){0, 0}, (vector2_t){64, 32});
    set_animation(image, create_animation(
        1,
        (vector2_t){
            100,
            0
        },
        (vector2_t){
            64,
            32
        },
        10000.0f,
        LINEAR
    ));
    menu_add_item(menu, item);
    menu_add_item(menu, item1);
    menu_add_item(menu, item2);
    menu_add_image(menu, image);
    item_set_on_click(item1, start_game);
    item_set_on_click(item2, leave_game);

    game_init(&game, engine.renderer);
    game.started = false;
    game_on_done(&game, goto_menu);

    long time_per_frame = 1000.0f / FPS;

    while (engine.is_running) {
        long first_frame_ticks = SDL_GetTicks();
        engine_process_events(&engine);
        if (game.started) {
            // game_debug(&game);
            if(!is_freezed(&game)) {
                game_update(&game, engine.delta_time, engine.key_manager);
            }
            game_render(&game, engine.renderer);
            for (int i = 0; i < engine.triggers->size; i++) {
                trigger_t *trigger = (trigger_t*) linked_list_get(engine.triggers, i);
                if (trigger->trigger_type == ENGINE_TRIGGER_GAME && trigger->valid()) {
                    trigger->cb();
                }
            }
        } else {
            menu_update(menu, SDL_GetTicks(), engine.key_manager);
            menu_render(menu, engine.renderer);
            for (int i = 0; i < engine.triggers->size; i++) {
                trigger_t *trigger = (trigger_t*) linked_list_get(engine.triggers, i);
                if (trigger->trigger_type == ENGINE_TRIGGER_MENU && trigger->valid()) {
                    trigger->cb();
                }
            }
        }

        long frame_time = SDL_GetTicks() - first_frame_ticks;
        if (frame_time < time_per_frame)
        {
            SDL_Delay(time_per_frame - frame_time);
            engine_set_delta_time(&engine, SPEED);
        }
        else
        {
            engine_set_delta_time(&engine, (float) frame_time / (float) time_per_frame * SPEED);
        }
    }

    game_destroy(&game);
    engine_destroy(&engine);
    SDL_Quit();
    return 0;
}
