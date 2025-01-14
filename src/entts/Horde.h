#pragma once

#include "screens/Game.h"

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

struct GAMESCOPE::Horde {
    enum State {
        DEPLOYING, ADVANCING, FROZEN
    };

    State state = DEPLOYING;
    Invader invaders[55];
    int alive_invaders_count;
    int i = 0, dx, dy;
    float time;

    void freeze();

    void draw();
    void update(float delta);
};
