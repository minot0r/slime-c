#include "../../include/objects/game/key_manager.h"

void key_manager_init(key_manager_t *key_manager)
{
    key_manager->key_z_down = false;
    key_manager->key_q_down = false;
    key_manager->key_d_down = false;
    key_manager->key_left_down = false;
    key_manager->key_right_down = false;
    key_manager->key_up_down = false;
}
void key_manager_process_events(key_manager_t *key_manager, SDL_Keycode key_code, bool is_down)
{
    switch (key_code)
    {
    case SDLK_z:
        key_manager->key_z_down = is_down;
        break;
    case SDLK_q:
        key_manager->key_q_down = is_down;
        break;
    case SDLK_d:
        key_manager->key_d_down = is_down;
        break;
    case SDLK_LEFT:
        key_manager->key_left_down = is_down;
        break;
    case SDLK_RIGHT:
        key_manager->key_right_down = is_down;
        break;
    case SDLK_UP:
        key_manager->key_up_down = is_down;
        break;
    case SDLK_SPACE:
        key_manager->key_space_down = is_down;
    default:
        break;
    }
}
void key_manager_update(key_manager_t *key_manager);