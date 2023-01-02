#include <stdio.h>

#include "../include/objects/engine/engine.h"
#include "../include/objects/game/game.h"
#include "../include/objects/menus/menu.h"
#include "../include/objects/menus/item.h"

#define SDL_MAIN_HANDLED
#define FPS 120

int main()
{
    engine_t engine;
    game_state_t game;
    menu_t* menu = menu_create(NULL, (SDL_Color){255, 0, 0, 255});
    item_t* item = item_create("Hello, World!", true, false, (vector2_t){0, 0}, 32);
    menu_add_item(menu, item);

    engine_init(&engine);
    game_init(&game, engine.renderer);

    float fpsWait = 1000.0f / FPS;

    game.started = true;

    while (engine.is_running)
    {
        uint32_t first_frame_ticks = SDL_GetTicks();
        engine_process_events(&engine);
        if (game.started) {
            // game_debug(&game);
            game_update(&game, engine.delta_time, engine.key_manager);
            game_check_collisions(&game);
            game_render(&game, engine.renderer);
        } else {
            menu_render(menu, engine.renderer);
        }
        int frame_time = SDL_GetTicks() - first_frame_ticks;
        if (frame_time < fpsWait)
        {
            SDL_Delay(fpsWait - frame_time);
        }
        engine_set_delta_time(&engine, (fpsWait - frame_time) / 1000.0f);
    }

    game_destroy(&game);
    engine_destroy(&engine);
    SDL_Quit();
    return 0;
}
