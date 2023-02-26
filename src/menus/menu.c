#include "../../include/objects/menus/menu.h"
#include "../../include/objects/menus/text_renderer.h"

#define STROKE_DELAY 150

menu_t *menu_create(SDL_Texture *background_image, SDL_Color background_color)
{
    menu_t *menu = malloc(sizeof(menu_t));
    menu->background_image = background_image;
    menu->background_color = background_color;
    menu->menu_items = linked_list_init();
    menu->menu_images = linked_list_init();
    menu->selected_item = -1;
    menu->last_stroke = STROKE_DELAY;
    return menu;
}

void menu_destroy(menu_t *menu) {
    for (int i = 0; i < menu->menu_items->size; i++) {
        item_t *item = linked_list_get(menu->menu_items, i);
        item_destroy(item);
    }
    for (int i = 0; i < menu->menu_images->size; i++) {
        image_t *image = linked_list_get(menu->menu_images, i);
        destroy_image(image);
    }
    linked_list_destroy(menu->menu_items);
    linked_list_destroy(menu->menu_images);
    free(menu);
}

void menu_update(menu_t *menu, unsigned long time_stroke, key_manager_t key_manager) {
    if (key_manager.key_up_down && time_stroke >= menu->last_stroke + STROKE_DELAY) {
        menu->last_stroke = time_stroke;
        do {
            menu->selected_item--;
            if (menu->selected_item < 0) {
                menu->selected_item = menu->menu_items->size - 1;
            }
        } while (!((item_t*) linked_list_get(menu->menu_items, menu->selected_item))->is_selectable);
    } else if(key_manager.key_right_down && time_stroke >= menu->last_stroke + STROKE_DELAY) {
        menu->last_stroke = time_stroke;
        item_click(((item_t*) linked_list_get(menu->menu_items, menu->selected_item)));
    }
    for (int i = 0; i < menu->menu_images->size; i++) {
        image_t *image = linked_list_get(menu->menu_images, i);
        image_update(image);
    }
}

void menu_render(menu_t *menu, engine_renderer_t *renderer) {
    SDL_SetRenderDrawColor(renderer->r_w, menu->background_color.r, menu->background_color.g, menu->background_color.b, menu->background_color.a);
    SDL_RenderFillRect(renderer->r_w, NULL);
    if (menu->background_image != NULL)
        SDL_RenderCopyEx(renderer->r_w, menu->background_image, NULL, NULL, 0, NULL, SDL_FLIP_NONE);

    for (int i = 0; i < menu->menu_items->size; i++) {
        item_t *item = linked_list_get(menu->menu_items, i);
        char *new_text = malloc(sizeof(char) * (strlen(item->text) + 2));
        strcpy(new_text, item->text);
        bool is_selected = menu->selected_item == i;
        if (is_selected)
            new_text = strcat(new_text, " >");
        render_text(
            renderer,
            new_text,
            item->position.x * renderer->scale,
            item->position.y * renderer->scale,
            item->size * renderer->scale,
            255,
            is_selected ? 10 : 255,
            is_selected ? 10 : 255,
            255);
        free(new_text);
    }
    for (int i = 0; i < menu->menu_images->size; i++) {
        image_t *image = linked_list_get(menu->menu_images, i);
        SDL_Rect rect = {
            .x = image->rect->x * renderer->scale,
            .y = image->rect->y * renderer->scale,
            .w = image->rect->w * renderer->scale,
            .h = image->rect->h * renderer->scale
        };
        SDL_RenderCopyEx(renderer->r_w, image->texture, NULL, &rect, 0, NULL, SDL_FLIP_NONE);
    }
    
    SDL_RenderPresent(renderer->r_w);
}

void menu_add_item(menu_t *menu, item_t *item) {
    if (menu->selected_item == -1) {
        menu->selected_item = menu->menu_items->size + 1;
    }
    linked_list_add(menu->menu_items, item);
}

void menu_add_image(menu_t *menu, image_t *image) {
    linked_list_add(menu->menu_images, image);
}