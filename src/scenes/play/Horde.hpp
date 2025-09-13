#pragma once

#include <pico.h>
#include <vector>
#include "Invader.hpp"

struct Bullet;

struct Horde {
    enum State {
        DEPLOYING, MARCHING
    } state;
    Invader invaders[55];
    int invaders_alive;
    int i;
    int sfx_i;
    float timer;
    float sfx_timer;

    Horde();
    int collide_rect(Pico_Rect rct, Pico_Anchor anc) const;
    int kill_invader(int i);
    Bullet *shoot(float ship_x);
    void update(float delta);
    void draw() const;
};
