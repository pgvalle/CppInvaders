#pragma once

struct UFO {
    enum State {
        AWAY, ALIVE, EXPLODING, DEAD
    } state;
    float x, vx;
    float timer;
    int score;

    UFO();
    void explode();
    void update(float delta);
    void draw() const;
};
