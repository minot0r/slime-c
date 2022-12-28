#include "../../include/objects/menus/item.h"

#include <string.h>
#include <stdlib.h>

item_t* item_create(char* text, bool is_selectable, bool is_selected, vector2_t position, int size) {
    item_t* item = malloc(sizeof(item_t));
    strcpy(item->text, text);
    item->is_selectable = is_selectable;
    item->is_selected = is_selected;
    item->position = position;
    item->size = size;
    return item;
}

void item_destroy(item_t* item) {
    free(item);
}