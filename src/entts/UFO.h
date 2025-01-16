#pragma once

struct UFO {
    enum State {
        AWAY, ALIVE, EXPLODING, DEAD
    };

    State state = AWAY;
    float x, vx, timer = 0;
    int score;

    void explode();

    void draw();
    void update(float delta);
};
