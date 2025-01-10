#pragma once

#include <pico.h>
#include <string>

struct Explosion
{
    int x, y;
    float lifetime;
    std::string img;
    SDL_Rect crop;

    void draw();
    void update(float delta);
};