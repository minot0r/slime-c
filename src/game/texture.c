#include "../../include/objects/game/texture.h"

SDL_Texture* load_texture(SDL_Renderer* renderer, const char* path) {
    SDL_Texture* texture = NULL;
    SDL_Surface* surface = IMG_Load(path);
    if (surface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (texture == NULL) {
            printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
        }
        SDL_FreeSurface(surface);
    }
    return texture;
}