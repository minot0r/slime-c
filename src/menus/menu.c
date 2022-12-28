#include "../../include/objects/menus/menu.h"
#include "../../include/objects/menus/text_renderer.h"

menu_t* menu_create(SDL_Texture* background_image, SDL_Color background_color) {
    menu_t* menu = malloc(sizeof(menu_t));
    menu->background_image = background_image;
    menu->background_color = background_color;
    menu->menu_items = linked_list_init();
    return menu;
}

void menu_destroy(menu_t* menu) {
    for(int i = 0; i < menu->menu_items->size; i++) {
        item_t* item = linked_list_get(menu->menu_items, i);
        item_destroy(item);
    }
    linked_list_destroy(menu->menu_items);
    free(menu);
}

void menu_render(menu_t* menu, SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, menu->background_color.r, menu->background_color.g, menu->background_color.b, menu->background_color.a);
    SDL_RenderFillRect(renderer, NULL);
    if(menu->background_image != NULL)
        SDL_RenderCopyEx(renderer, menu->background_image, NULL, NULL, 0, NULL, SDL_FLIP_NONE);
    
    for(int i = 0; i < menu->menu_items->size; i++) {
        item_t* item = linked_list_get(menu->menu_items, i);
        render_text(
            renderer,
            item->text,
            item->position.x,
            item->position.y,
            item->size,
            255,
            item->is_selected ? 0 : 255,
            item->is_selected ? 0 : 255,
            255
        );
    }
    SDL_RenderPresent(renderer);
}
void menu_add_item(menu_t* menu, item_t* item) {
    linked_list_add(menu->menu_items, item);
}