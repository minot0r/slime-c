#include "../../include/objects/engine/engine.h"
#include <SDL2/SDL_ttf.h>

#include <stdlib.h>
#include <time.h>

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
        640, 480,
        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
    );
    
    if(engine->window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }

    engine->renderer = SDL_CreateRenderer(engine->window, -1, SDL_RENDERER_SOFTWARE);

    if(engine->renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return;
    }


    engine->is_running = true;
    engine->delta_time = 0;
}

void engine_destroy(engine_t* engine) {
    SDL_DestroyRenderer(engine->renderer);
    SDL_DestroyWindow(engine->window);
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
