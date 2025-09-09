#pragma once

#include <pico.h>

struct Invader {
    static int counter;

    enum State {
        UP, DOWN, DYING, DEAD
    } state;
    int type;
    int x, y;

    Invader();
    Pico_Rect get_rect() const;
    const char* get_image() const;
    bool is_alive() const;
    void move(int dx, int dy);
    void draw() const;
};
