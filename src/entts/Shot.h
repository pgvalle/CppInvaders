#pragma once

#include <string>

struct Shot {
    enum State { ALIVE, EXPLODING, DEAD };

    State state = ALIVE;
    float x, y, vy;
    float explosion_time = 0;

    void draw();
    void update(float delta);
};
