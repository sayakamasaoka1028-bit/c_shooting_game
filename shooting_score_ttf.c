#include "shooting_score_ttf.h"
#include <stdio.h>

void draw_score(SDL_Renderer *renderer, TTF_Font *font, int score) {
    char buf[32];
    snprintf(buf, sizeof(buf), "Score: %d", score);

    SDL_Color color = {255, 255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, buf, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dst = {10, 10, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
