#include "../../include/objects/menus/text_renderer.h"

#include <SDL2/SDL_ttf.h>

void render_text(engine_renderer_t* renderer, const char *text, int x, int y, int size, int r, int g, int b, int a) {
    TTF_Font* font = TTF_OpenFont("/home/valentin/Perso/slime/assets/fonts/PressStart2P.ttf", size);
    /* SDL_Color color = {r, g, b, a};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
 */
    if(!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(-1);
    }
    SDL_Color textColor = {r, g, b, a};
    SDL_Surface* surface = TTF_RenderText_Solid(font, text, textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer->r_w, surface);
    if(!texture) {
        printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
        exit(-1);
    }
    SDL_Rect rect = {x * renderer->scale, y * renderer->scale,
    surface->w * renderer->scale, surface->h * renderer->scale};
    SDL_RenderCopy(renderer->r_w, texture, NULL, &rect);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}