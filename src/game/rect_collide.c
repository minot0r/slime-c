#include "../../include/objects/game/rect_collide.h"

#include <stdlib.h>

rect_collide_t *create_rect_collide(char name[32], int x, int y, int width, int height, collide_type_e collide_type, color_t color, bool transparent) {
    rect_collide_t *rect_collide = malloc(sizeof(rect_collide_t));
    strcpy(rect_collide->name, name);
    rect_collide->position = (vector2_t) { x, y };
    rect_collide->width = width;
    rect_collide->height = height;
    rect_collide->collide_type = collide_type;
    rect_collide->color = color;
    rect_collide->transparent = transparent;
    return rect_collide;
}

void render_rect_collide(rect_collide_t *rect_collide, SDL_Renderer *renderer) {
    if (rect_collide->transparent) return;
    SDL_SetRenderDrawColor(renderer, rect_collide->color.r, rect_collide->color.g, rect_collide->color.b, rect_collide->color.a);
    SDL_Rect rect = { rect_collide->position.x, rect_collide->position.y, rect_collide->width, rect_collide->height };
    SDL_RenderFillRect(renderer, &rect);
}

void destroy_rect_collide(rect_collide_t *rect_collide) {
    free(rect_collide);
}

collide_dir_e is_colliding_y(rect_collide_t *rect_collide_1, vector2_t pos, int width, int height) {
    if(rect_collide_1->collide_type == OUTSIDE) {
        /* if (pos.x + width < rect_collide_1->position.x) return NONE;
        if (pos.x > rect_collide_1->position.x + rect_collide_1->width) return NONE;
        if (pos.y + height < rect_collide_1->position.y) return NONE;
        if (pos.y > rect_collide_1->position.y + rect_collide_1->height) return NONE;
        if (pos.x >= rect_collide_1->position.x) return LEFT;
        if (pos.x + width <= rect_collide_1->position.x + rect_collide_1->width) return RIGHT;
        if (pos.y >= rect_collide_1->position.y) return BOTTOM;
        if (pos.y + height <= rect_collide_1->position.y + rect_collide_1->height) return TOP; */
        // if collide with left side

        // if collide with top side
        if (pos.y + height >= rect_collide_1->position.y && pos.y + height <= rect_collide_1->position.y + rect_collide_1->height) {
            if (pos.x + width >= rect_collide_1->position.x && pos.x <= rect_collide_1->position.x + rect_collide_1->width) {
                return TOP;
            }
        }
        // if collide with bottom side
        if (pos.y >= rect_collide_1->position.y && pos.y <= rect_collide_1->position.y + rect_collide_1->height) {
            if (pos.x + width >= rect_collide_1->position.x && pos.x <= rect_collide_1->position.x + rect_collide_1->width) {
                return BOTTOM;
            }
        }
    } else if(rect_collide_1->collide_type == INSIDE) {
        if (pos.y <= rect_collide_1->position.y) return TOP;
        if (pos.y + height >= rect_collide_1->position.y + rect_collide_1->height) return BOTTOM;
        return NONE;
    }
    return NONE;
}

collide_dir_e is_colliding_x(rect_collide_t *rect_collide_1, vector2_t pos, int width, int height) {
    if(rect_collide_1->collide_type == OUTSIDE) {
        // if collide with left side
        if (pos.x + width >= rect_collide_1->position.x && pos.x + width <= rect_collide_1->position.x + rect_collide_1->width) {
            if (pos.y + height >= rect_collide_1->position.y && pos.y <= rect_collide_1->position.y + rect_collide_1->height) {
                return LEFT;
            }
        }
        // if collide with right side
        if (pos.x >= rect_collide_1->position.x && pos.x <= rect_collide_1->position.x + rect_collide_1->width) {
            if (pos.y + height >= rect_collide_1->position.y && pos.y <= rect_collide_1->position.y + rect_collide_1->height) {
                return RIGHT;
            }
        }
    } else if(rect_collide_1->collide_type == INSIDE) {
        if (pos.x <= rect_collide_1->position.x) return LEFT;
        if (pos.x + width >= rect_collide_1->position.x + rect_collide_1->width) return RIGHT;
        return NONE;
    }
    return NONE;
}