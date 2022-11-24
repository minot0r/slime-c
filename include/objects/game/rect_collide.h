#ifndef RECT_COLLIDE_H
#define RECT_COLLIDE_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "../math/vector2.h"
#include "../other/color.h"

typedef enum {
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
} collide_dir_e;

typedef enum {
    INSIDE,
    OUTSIDE
} collide_type_e;

typedef struct {
    vector2_t position;
    int width;
    int height;
    collide_type_e collide_type;
    color_t color;
    bool transparent;
    char name[32];
} rect_collide_t;


rect_collide_t *create_rect_collide(char name[32], int x, int y, int width, int height, collide_type_e collide_type, color_t color, bool transparent);
void render_rect_collide(rect_collide_t *rect_collide, SDL_Renderer *renderer);
void destroy_rect_collide(rect_collide_t *rect_collide);
collide_dir_e is_colliding_y(rect_collide_t *rect_collide_1, vector2_t pos, int width, int height);
collide_dir_e is_colliding_x(rect_collide_t *rect_collide_1, vector2_t pos, int width, int height);

#endif