#include "../../include/objects/engine/engine.h"
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <time.h>

engine_renderer_t* engine_renderer_init() {
    engine_renderer_t* renderer = malloc(sizeof(engine_renderer_t));
    if(renderer == NULL)
        return NULL;
    renderer->r_w = NULL;
    renderer->scale = 1;
    return renderer;
}

void engine_init(engine_t* engine) {
    srand(time(NULL));
    
    engine->is_running = false;
    key_manager_init(&engine->key_manager);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    if(TTF_Init() < 0) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    engine->window = SDL_CreateWindow(
        "SDL2 OpenGL", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        ENGINE_RESOLUTION_X, ENGINE_RESOLUTION_Y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN 
        | SDL_WINDOW_RESIZABLE | SDL_WINDOW_FULLSCREEN_DESKTOP
    );
    
    if(engine->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    engine->renderer = engine_renderer_init();
    if(engine->renderer == NULL) {
        printf("Engine renderer could not be created!\n");
        return;
    }

    engine->renderer->r_w = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_SOFTWARE);

    if(engine->renderer->r_w == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    int pixels_width, pixels_height, pixels_per_unit_x, pixels_per_unit_y;
    SDL_GetWindowSize(engine->window, &pixels_width, &pixels_height);
    pixels_per_unit_x = pixels_width / ENGINE_RESOLUTION_X;
    pixels_per_unit_y = pixels_height / ENGINE_RESOLUTION_Y;

    // change pixer per unit to min of x and y
    if(pixels_per_unit_x > pixels_per_unit_y) {
        engine->renderer->scale = pixels_per_unit_x;
    } else {
        engine->renderer->scale = pixels_per_unit_y;
    }
    
    printf("Pixels per unit x-y: %d\n", engine->renderer->scale);


    engine->is_running = true;
    engine->delta_time = 0.0f;

    engine->triggers = linked_list_init();
}

void engine_destroy(engine_t* engine) {
    SDL_DestroyRenderer(engine->renderer->r_w);
    free(engine->renderer);
    SDL_DestroyWindow(engine->window);
    for(int i = 0; i < engine->triggers->size; i++) {
        trigger_t* trigger = linked_list_get(engine->triggers, i);
        free(trigger);
    }
    linked_list_destroy(engine->triggers);
    TTF_Quit();
}

void engine_process_events(engine_t* engine) {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            engine->is_running = false;
            break;
        case SDL_KEYDOWN:
            key_manager_process_events(&engine->key_manager, event.key.keysym.sym, true);
            break;
        case SDL_KEYUP:
            key_manager_process_events(&engine->key_manager, event.key.keysym.sym, false);
            break;
        default:
            break;
    }
}

void engine_set_delta_time(engine_t* engine, float delta_time) {
    engine->delta_time = delta_time;
}

void engine_set_trigger(engine_t* engine, trigger_type_t trigger_type, trigger_validator valid, trigger_callback cb) {
    trigger_t* trigger = malloc(sizeof(trigger_t));
    trigger->trigger_type = trigger_type;
    trigger->valid = valid;
    trigger->cb = cb;
    linked_list_add(engine->triggers, trigger);
}