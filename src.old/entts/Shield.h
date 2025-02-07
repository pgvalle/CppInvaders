#pragma once

#include "Shot.h"
#include <pico.h>

struct Shield {
    int x;
    bool bits[352];

    void deploy(int x);
    bool damage(Shot *shot);
    void damage(SDL_Rect rect);

    void draw();
};