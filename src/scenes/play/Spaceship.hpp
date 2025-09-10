#pragma once

#include <pico.h>

struct Bullet;

struct Spaceship {
    enum State {
        DEPLOYING, DEPLOYED, EXPLODING
    } state;
    float x;
    float timer;
    int explosion_frames;

    Spaceship();
    bool collide_rect(Pico_Rect rct, Pico_Anchor anc) const;
    void explode();
    Bullet* shoot();
    void update(float delta);
    void draw() const;
};
