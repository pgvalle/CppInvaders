#pragma once

#include <pico.h>

struct Shot {
    float x, y, vy;

    void draw();
    void update(float delta);
};
