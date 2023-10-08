#include <stdio.h>

#include "../include/objects/engine/engine.h"
#include "../include/objects/game/texture.h"
#include "../include/objects/game/game.h"
#include "../include/objects/menus/menu.h"
#include "../include/objects/menus/item.h"
#include "../include/objects/menus/text_renderer.h"

#include <SDL2/SDL_mixer.h>

#define SDL_MAIN_HANDLED
#define FPS 60.0f
#define SPEED 0.9f

engine_t engine;
game_state_t game;
menu_t *menu;
menu_t *menu_home;
menu_t *menu_sound;
item_t *item_sound_indicator;
int game_sound = 100;
bool starting_game = false;

void start_game() {
    game.started = true;
    Mix_PauseMusic();
    set_freeze(&game, 1);
    game_render(&game, engine.renderer);
}

void change_volume() {
    game_sound += 10;
    if (game_sound > 100) game_sound = 0;
    Mix_VolumeMusic(game_sound);
    char str[32];
    sprintf(str, "%d%%", game_sound);
    item_set_text(item_sound_indicator, str);
}

void settings() {
    menu_sound->last_stroke = SDL_GetTicks(); // to avoid combo selecting between both menu
    menu = menu_sound;
}

void home() {
    menu_home->last_stroke = SDL_GetTicks(); // to avoid combo selecting between both menu
    menu = menu_home;
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
    Mix_RewindMusic();
    Mix_ResumeMusic();
}

int main() {
    engine_init(&engine);

    menu_home = menu_create(NULL, (SDL_Color){0, 0, 0, 255});
    item_t *item = item_create("SLIME", false, (vector2_t){200, 25}, 50);
    item_t *item1 = item_create("Commencer une partie", true, (vector2_t){25, 200}, 24);
    item_t *item2 = item_create("Reglages", true, (vector2_t){25, 250}, 24);
    item_t *item3 = item_create("Valentin Giorgetti", false, (vector2_t){75, 400}, 11);
    item_t *item4 = item_create("Nemo Demarquay", false, (vector2_t){325, 400}, 11);
    item_t *item5 = item_create("Younes Saadaoui", false, (vector2_t){75, 425}, 11);
    item_t *item6 = item_create("Daham Karunanayake", false, (vector2_t){325, 425}, 11);

    menu_add_item(menu_home, item);
    menu_add_item(menu_home, item1);
    menu_add_item(menu_home, item2);
    menu_add_item(menu_home, item3);
    menu_add_item(menu_home, item4);
    menu_add_item(menu_home, item5);
    menu_add_item(menu_home, item6);
    item_set_on_click(item1, start_game);
    item_set_on_click(item2, settings);

    menu = menu_home;

    menu_sound = menu_create(NULL, (SDL_Color){0, 0, 0, 255});
    item_t *item_sound = item_create("Baisser le son", false, (vector2_t){120, 25}, 25);
    item_sound_indicator = item_create("100%", true, (vector2_t){25, 200}, 24);
    item_t *item_sound_back = item_create("Retour", true, (vector2_t){25, 250}, 24);

    menu_add_item(menu_sound, item_sound);
    menu_add_item(menu_sound, item_sound_indicator);
    menu_add_item(menu_sound, item_sound_back);
    item_set_on_click(item_sound_back, home);
    item_set_on_click(item_sound_indicator, change_volume);

/*     image_t *image = create_image(load_texture(engine.renderer, "assets/slime/slime_dino.png"), (vector2_t){0, 0}, (vector2_t){64, 32});
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
    )); */

    game_init(&game, engine.renderer);
    game_on_done(&game, goto_menu);

    Mix_Music *music = Mix_LoadMUS("assets/sound/8bit.mp3");
    Mix_PlayMusic(music, -1);

    long time_per_frame = 1000.0f / FPS;

    while (engine.is_running) {
        long first_frame_ticks = SDL_GetTicks();
        engine_process_events(&engine);
        if (game.started) {
            // game_debug(&game);
            if(!is_freezed(&game)) game_update(&game, engine.delta_time, engine.key_manager);
            if(!is_freezed(&game)) game_render(&game, engine.renderer);
            for (int i = 0; i < engine.triggers->size; i++) {
                trigger_t *trigger = (trigger_t*) linked_list_get(engine.triggers, i);
                if (trigger->trigger_type == ENGINE_TRIGGER_GAME && trigger->valid()) {
                    trigger->cb();
                }
            }
        } else {
            if(!is_freezed(&game)) menu_update(menu, SDL_GetTicks(), engine.key_manager);
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
