#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "../other/linkedlist.h"
#include "key_manager.h"

// resolution = 5:4
#define ENGINE_RESOLUTION_X 625
#define ENGINE_RESOLUTION_Y 500
#define ENGINE_RESOLUTION_RATIO_X 1
#define ENGINE_RESOLUTION_RATIO_Y 1

typedef enum {
    ENGINE_TRIGGER_MENU,
    ENGINE_TRIGGER_GAME
} trigger_type_t;

typedef bool (*trigger_validator)(void);
typedef void (*trigger_callback)(void);

typedef struct {
    trigger_type_t trigger_type;
    trigger_validator valid;
    trigger_callback cb;
} trigger_t;

typedef struct engine_renderer_t {
    SDL_Renderer* r_w;
    int scale;
} engine_renderer_t;

typedef struct engine_t {
    engine_renderer_t* renderer;
    SDL_Window* window;
    key_manager_t key_manager;
    float delta_time;
    bool is_running;
    linked_list_t* triggers;
} engine_t;

engine_renderer_t* engine_renderer_init();
void engine_init(engine_t* engine);
void engine_destroy(engine_t* engine);
void engine_process_events(engine_t* engine);
void engine_set_delta_time(engine_t* engine, float delta_time);
void engine_set_trigger(engine_t* engine, trigger_type_t trigger_type, trigger_validator valid, trigger_callback cb);

#endif