#include "../../include/objects/menus/item.h"

#include <string.h>
#include <stdlib.h>

item_t* item_create(
    char* text,
    bool is_selectable,
    vector2_t position,
    int size
) {
    item_t* item = malloc(sizeof(item_t));
    strcpy(item->text, text);
    item->is_selectable = is_selectable;
    item->position = position;
    item->size = size;
    item->on_click_cb = NULL;
    return item;
}

item_t* item_create_clickable(
    char* text,
    bool is_selectable,
    vector2_t position,
    int size,
    on_click_callback_t on_click_cb
) {
    item_t* item = malloc(sizeof(item_t));
    strcpy(item->text, text);
    item->is_selectable = is_selectable;
    item->position = position;
    item->size = size;
    item->on_click_cb = on_click_cb;
    return item;
}

void item_set_on_click(item_t* item, on_click_callback_t on_click_cb) {
    item->on_click_cb = on_click_cb;
}

void item_destroy(item_t* item) {
    free(item);
}

void item_click(item_t* item) {
    if(item->is_selectable) {
        if(item->on_click_cb != NULL)
            item->on_click_cb();
    }
}