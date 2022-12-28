#include <stdio.h>

#include "../include/objects/game/game.h"
#include "../include/objects/menus/menu.h"
#include "../include/objects/menus/item.h"

#define SDL_MAIN_HANDLED
#define FPS 120

int main()
{

    game_state_t game;
    menu_t* menu = menu_create(NULL, (SDL_Color){255, 0, 0, 255});
    item_t* item = item_create("Hello, World!", true, false, (vector2_t){0, 0}, 32);
    menu_add_item(menu, item);

    game_init(&game);

    float fpsWait = 1000.0f / FPS;

    while (game.is_running)
    {
        uint32_t first_frame_ticks = SDL_GetTicks();
        game_process_events(&game);
        if (game.started) {
            // game_debug(&game);
            game_update(&game);
            game_check_collisions(&game);
            game_render(&game);
        } else {
            menu_render(menu, game.renderer);
        }
        int frame_time = SDL_GetTicks() - first_frame_ticks;
        if (frame_time < fpsWait)
        {
            SDL_Delay(fpsWait - frame_time);
        }
        game_set_delta_time(&game, (fpsWait - frame_time) / 1000.0f);
    }

    game_destroy(&game);
    SDL_Quit();
    return 0;
}
