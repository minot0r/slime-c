#include "../../include/objects/game/rect_collide.h"

#include <stdlib.h>

rect_collide_t *create_rect_collide(char name[32], int x, int y, int width, int height, collide_e colide_type, color_t color, bool transparent) {
    rect_collide_t *rect_collide = malloc(sizeof(rect_collide_t));
    strcpy(rect_collide->name, name);
    rect_collide->position = (vector2_t) { x, y };
    rect_collide->width = width;
    rect_collide->height = height;
    rect_collide->colide_type = colide_type;
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