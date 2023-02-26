#ifndef MENU_H
#define MENU_H

#include "../engine/engine.h"
#include "../engine/key_manager.h"
#include "../other/linkedlist.h"
#include "item.h"
#include "image.h"

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* background_image;
    SDL_Color background_color;
    linked_list_t* menu_items;
    linked_list_t* menu_images;
    int selected_item;
    unsigned long last_stroke;
} menu_t;

menu_t* menu_create(SDL_Texture* background_image, SDL_Color background_color);
void menu_destroy(menu_t* menu);
void menu_update(menu_t* menu, unsigned long time_stroke, key_manager_t key_manager);
void menu_render(menu_t* menu, engine_renderer_t* renderer);
void menu_add_item(menu_t* menu, item_t* item);
void menu_add_image(menu_t* menu, image_t* image);

#endif