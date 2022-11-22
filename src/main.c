#include <stdio.h>

#include "../include/objects/game/game.h"

#define SDL_MAIN_HANDLED
#define FPS 60

int main() {
    
    game_state_t game;

    game_init(&game);

    float fpsWait = 1000.0f / FPS;

    while(game.is_running) {
        uint32_t first_frame_ticks = SDL_GetTicks();
        game_process_events(&game);
        game_debug(&game);
        game_update(&game);
        game_check_collisions(&game);
        game_render(&game);
        int frame_time = SDL_GetTicks() - first_frame_ticks;
        if (frame_time < fpsWait) {
            SDL_Delay(fpsWait - frame_time);
        }
        game_set_delta_time(&game, (fpsWait - frame_time) / 1000.0f);
    }

    game_destroy(&game);

    return 0;
}
