#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "key_manager.h"

typedef struct engine_t {
    SDL_Renderer* renderer;
    SDL_Window* window;
    key_manager_t key_manager;
    float delta_time;
    bool is_running;
} engine_t;

void engine_init(engine_t* engine);
void engine_destroy(engine_t* engine);
void engine_process_events(engine_t* engine);
void engine_set_delta_time(engine_t* engine, float delta_time);

#endif