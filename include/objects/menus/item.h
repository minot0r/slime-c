#ifndef ITEM_H
#define ITEM_H

#include "../math/vector2.h"

#include <stdbool.h>

typedef struct {
    char text[32];
    bool is_selectable;
    bool is_selected;
    vector2_t position;
    int size;
} item_t;

item_t* item_create(char* text, bool is_selectable, bool is_selected, vector2_t position, int size);
void item_destroy(item_t* item);

# endif