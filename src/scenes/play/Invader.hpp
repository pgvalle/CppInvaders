#pragma once

#include <pico.h>

struct Invader {
    static int counter;

    enum State {
        UP, DOWN, DEAD
    } state;
    int type;
    int x, y;

    Invader();
    Pico_Rect get_rect() const;
    void move(int dx, int dy);
    void draw() const;
};
