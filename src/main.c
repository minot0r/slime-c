#include <stdio.h>

#include "../include/objects/engine/engine.h"
#include "../include/objects/game/game.h"
#include "../include/objects/menus/menu.h"
#include "../include/objects/menus/item.h"

#define SDL_MAIN_HANDLED
#define FPS 120
#define GAME_SPEED 0.65f

engine_t engine;
game_state_t game;
menu_t *menu;

void start_game()
{
    game.started = true;
}

void leave_game()
{
    engine.is_running = false;
}

int main()
{

    menu = menu_create(NULL, (SDL_Color){0, 0, 0, 255});
    item_t *item = item_create("SLIME", false, (vector2_t){0, 0}, 32);
    item_t *item1 = item_create("Commencer une partie", true, (vector2_t){0, 64}, 24);
    item_t *item2 = item_create("Quitter", true, (vector2_t){0, 96}, 24);
    menu_add_item(menu, item);
    menu_add_item(menu, item1);
    menu_add_item(menu, item2);
    item_set_on_click(item1, start_game);
    item_set_on_click(item2, leave_game);

    engine_init(&engine);
    game_init(&game, engine.renderer);

    game.started = false;

    float time_per_frame = 1000.0f / FPS;

    while (engine.is_running)
    {
        uint32_t first_frame_ticks = SDL_GetTicks();
        engine_process_events(&engine);
        if (game.started)
        {
            // game_debug(&game);
            game_update(&game, engine.delta_time, engine.key_manager);
            game_check_collisions(&game);
            game_render(&game, engine.renderer);
        }
        else
        {
            menu_update(menu, SDL_GetTicks(), engine.key_manager);
            menu_render(menu, engine.renderer);
        }
        int frame_time = SDL_GetTicks() - first_frame_ticks;
        if (frame_time < time_per_frame)
        {
            SDL_Delay(time_per_frame - frame_time);
            engine_set_delta_time(&engine, time_per_frame / 1000.0f * GAME_SPEED);
        }
        else
        {
            engine_set_delta_time(&engine, frame_time / 1000.0f * GAME_SPEED);
        }
    }

    game_destroy(&game);
    engine_destroy(&engine);
    SDL_Quit();
    return 0;
}
