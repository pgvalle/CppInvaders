#pragma once

#include "screens/Game.h"

struct GAMESCOPE::UFO {
    enum State {
        AWAY, ALIVE, EXPLODING, DEAD
    };

    State state = AWAY;
    float x, vx, time = 0;
    int score;

    void explode();

    void draw();
    void update(float delta);
};
