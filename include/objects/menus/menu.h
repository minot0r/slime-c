#ifndef MENU_H
#define MENU_H

#include "item.h"
#include "../other/linkedlist.h"

#include <SDL2/SDL.h>

typedef struct {
    SDL_Texture* background_image;
    SDL_Color background_color;
    linked_list_t* menu_items;
} menu_t;

menu_t* menu_create(SDL_Texture* background_image, SDL_Color background_color);
void menu_destroy(menu_t* menu);
void menu_render(menu_t* menu, SDL_Renderer* renderer);
void menu_add_item(menu_t* menu, item_t* item);

#endif