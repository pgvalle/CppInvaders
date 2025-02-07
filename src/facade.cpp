#include "facade.h"
#include <string>
#include <unordered_map>

static std::unordered_map<std::string, SDL_Texture *> img;
static std::unordered_map<std::string, Mix_Chunk *> sfx;

SDL_Window *win;
SDL_Renderer *ren;
TTF_Font *fnt;

void fcd_init() {
    win = SDL_CreateWindow(
        "CppInvaders - sdl",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        448,
        512,
        0
    );

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_RenderSetScale(ren, 2, 2);

    fnt = TTF_OpenFont("./res/font.ttf", 8);
}

void fcd_quit() {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
}

void fcd_draw_text(int x, int y, const char *text) {
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(ren, &r, &g, &b, &a);

    SDL_Surface* sfc = TTF_RenderText_Blended(fnt, text, (SDL_Color){r,g,b,a});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, sfc);
    SDL_Rect rct = {x, y, sfc->w, sfc->h};

    SDL_RenderCopy(ren, tex, NULL, &rct);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(sfc);
}

void fcd_draw_image(int x, int y, const char *image, const SDL_Rect &crop) {
    if (img.find(image) == img.end()) {
        SDL_Surface *sfc = IMG_Load(image);
        SDL_assert(sfc);

        SDL_Texture *tex = SDL_CreateTextureFromSurface(ren, sfc);
        img[image] = tex;
    }

    SDL_Rect r = {x, y, crop.w, crop.h};
    SDL_RenderCopy(ren, img.at(image), &crop, &r);
}

void fcd_play_sound(const char *sound) {
    if (sfx.find(sound) == sfx.end()) {
        sfx[sound] = Mix_LoadWAV(sound);
        SDL_assert(sfx[sound]);
    }

    Mix_PlayChannel(-1, sfx.at(sound), 0);
}
