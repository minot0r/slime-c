#include "../../include/objects/menus/menu.h"
#include "../../include/objects/menus/text_renderer.h"

#define STROKE_DELAY 100

menu_t *menu_create(SDL_Texture *background_image, SDL_Color background_color)
{
    menu_t *menu = malloc(sizeof(menu_t));
    menu->background_image = background_image;
    menu->background_color = background_color;
    menu->menu_items = linked_list_init();
    menu->selected_item = 0;
    menu->last_stroke = STROKE_DELAY;
    return menu;
}

void menu_destroy(menu_t *menu)
{
    for (int i = 0; i < menu->menu_items->size; i++)
    {
        item_t *item = linked_list_get(menu->menu_items, i);
        item_destroy(item);
    }
    linked_list_destroy(menu->menu_items);
    free(menu);
}

void menu_update(menu_t *menu, unsigned long time_stroke, key_manager_t key_manager)
{
    printf("time_stroke: %lu\n", time_stroke);
    int selectable_item_length = 0;
    for (int i = 0; i < menu->menu_items->size; i++)
    {
        item_t *item = linked_list_get(menu->menu_items, i);
        if (item->is_selectable)
            selectable_item_length++;
    }
    item_t **selectable_items = malloc(sizeof(item_t *) * selectable_item_length);
    int selectable_item_index = 0;
    for (int i = 0; i < menu->menu_items->size; i++)
    {
        item_t *item = linked_list_get(menu->menu_items, i);
        if (item->is_selectable)
        {
            selectable_items[selectable_item_index] = item;
            selectable_item_index++;
        }
    }

    if (key_manager.key_up_down && time_stroke >= menu->last_stroke + STROKE_DELAY)
    {
        menu->last_stroke = time_stroke;
        ((item_t *)linked_list_get(menu->menu_items, menu->selected_item))->is_selected = false;
        menu->selected_item--;
        if (menu->selected_item < 0)
        {
            menu->selected_item = selectable_item_length - 1;
        }
        ((item_t *)linked_list_get(menu->menu_items, menu->selected_item))->is_selected = true;
    }
    printf("menu->selected_item: %d\n", menu->selected_item);
}

void menu_render(menu_t *menu, engine_renderer_t *renderer)
{
    SDL_SetRenderDrawColor(renderer->r_w, menu->background_color.r, menu->background_color.g, menu->background_color.b, menu->background_color.a);
    SDL_RenderFillRect(renderer->r_w, NULL);
    if (menu->background_image != NULL)
        SDL_RenderCopyEx(renderer->r_w, menu->background_image, NULL, NULL, 0, NULL, SDL_FLIP_NONE);

    for (int i = 0; i < menu->menu_items->size; i++)
    {
        item_t *item = linked_list_get(menu->menu_items, i);
        char *new_text = malloc(sizeof(char) * (strlen(item->text) + 2));
        strcpy(new_text, item->text);
        if (item->is_selected)
            new_text = strcat(new_text, " >");
        render_text(
            renderer,
            new_text,
            item->position.x * renderer->scale,
            item->position.y * renderer->scale,
            item->size * renderer->scale,
            255,
            item->is_selected ? 10 : 255,
            item->is_selected ? 10 : 255,
            255);
        free(new_text);
    }
    SDL_RenderPresent(renderer->r_w);
}
void menu_add_item(menu_t *menu, item_t *item)
{
    linked_list_add(menu->menu_items, item);
}