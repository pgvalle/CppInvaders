#pragma once

#include <pico.h>
#include "Invader.hpp"

struct Bullet;

struct Horde {
    enum State {
        PREDEPLOY, DEPLOYING, MARCHING, FROZEN
    } state;
    Invader invaders[55];
    int i, dying_inv_i;
    int sfx_i;
    int dx, dy;
    float timer;
    float sfx_timer;

    Horde();
    int collide_rect(Pico_Rect rct, Pico_Anchor anc) const;
    void kill_invader(int i);
    Bullet *shoot(float ship_x);
    void update(float delta);
    void draw() const;
};
