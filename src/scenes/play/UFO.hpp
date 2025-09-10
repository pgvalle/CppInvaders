#pragma once

#include <pico.h>

struct UFO {
    enum State {
        AWAY, ALIVE, EXPLODING, DEAD
    } state;
    float x, vx;
    float timer;
    int score;

    UFO();
    void explode();
    bool collide_rect(Pico_Rect rct, Pico_Anchor anc) const;
    void update(float delta);
    void draw() const;
};
