#pragma once

#include <pico.h>

struct Invader {
    static int count;
    static int dx, dy;

    enum State {
        UP, DOWN, DYING, DEAD
    } state;
    int type;
    int x, y;
    float timer;

    Invader();
    Pico_Rect get_rect() const;
    const char* get_image() const;
    bool is_alive() const;
    void kill();
    void update(float delta);
    void draw() const;
};
