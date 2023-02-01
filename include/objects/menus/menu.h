#ifndef MENU_H
#define MENU_H

#include "item.h"
#include "../engine/engine.h"
#include "../engine/key_manager.h"
#include "../other/linkedlist.h"

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* background_image;
    SDL_Color background_color;
    linked_list_t* menu_items;
    int selected_item;
    unsigned long last_stroke;
} menu_t;

menu_t* menu_create(SDL_Texture* background_image, SDL_Color background_color);
void menu_destroy(menu_t* menu);
void menu_update(menu_t* menu, unsigned long time_stroke, key_manager_t key_manager);
void menu_render(menu_t* menu, engine_renderer_t* renderer);
void menu_add_item(menu_t* menu, item_t* item);
on_click_callback_t menu_open(menu_t* menu);

#endif