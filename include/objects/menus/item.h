#ifndef ITEM_H
#define ITEM_H

#include "../math/vector2.h"

#include <stdbool.h>

typedef void (*on_click_callback_t)(void);

typedef struct {
    char text[32];
    bool is_selectable;
    vector2_t position;
    int size;
    on_click_callback_t on_click_cb;
} item_t;

item_t* item_create_clickable(char* text, bool is_selectable, vector2_t position, int size, on_click_callback_t on_click_cb);
item_t* item_create(char* text, bool is_selectable, vector2_t position, int size);
void item_set_on_click(item_t* item, on_click_callback_t on_click_cb);
void item_destroy(item_t* item);
void item_click(item_t* item);

# endif