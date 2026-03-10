#pragma once

#include <pico.h>
#include "Bullet.hpp"

struct Shield {
    int x;
    bool bits[352];

    void deploy(int x);
    bool damage(Bullet *shot);
    void damage(SDL_Rect rect);

    void draw() const;
};
