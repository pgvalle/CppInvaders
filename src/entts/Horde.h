#pragma once

#include "Shot.h"
#include <pico.h>
#include <vector>

struct Invader {
    enum State {
        UP, DOWN, DEAD
    };

    State state = DEAD;
    int type, x, y;

    SDL_Rect get_rect();
    void deploy(int i);
    void move(int dx, int dy);

    void draw();
};

struct Horde {
    enum State {
        DEPLOYING, MARCHING, FROZEN
    };

    State state = DEPLOYING;
    Invader invaders[55];
    int i = 0, index_dying_invader;
    int dx, dy;
    float time;

    std::vector<int> get_alive_invaders();
    void explode_invader(int index);
    Shot *shoot(float spaceship_x);

    void draw();
    void update(float delta);
};
