#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL2_gfxPrimitives.h>

extern SDL_Window *win;
extern SDL_Renderer *ren;
extern TTF_Font *fnt;

void fcd_init();
void fcd_quit();
void fcd_draw_text(int x, int y, const char *text);
void fcd_draw_image(int x, int y, const char *image, const SDL_Rect &crop);
void fcd_play_sound(const char *sound);