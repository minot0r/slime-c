#ifndef KEY_MANAGER_H
#define KEY_MANAGER_H

#include <stdbool.h>

#include <SDL2/SDL.h>

typedef struct {
    bool key_z_down;
    bool key_q_down;
    bool key_d_down;
    bool key_left_down;
    bool key_right_down;
    bool key_up_down;
    bool key_space_down;
} key_manager_t;

void key_manager_init(key_manager_t *key_manager);
void key_manager_process_events(key_manager_t *key_manager, SDL_Keycode key_code, bool is_down);
void key_manager_update(key_manager_t *key_manager);

#endif