#pragma once

#include "Invader.hpp"

struct Bullet;

struct Horde {
    enum State {
        PREDEPLOY, DEPLOYING, MARCHING, FROZEN
    } state;
    Invader invaders[55];
    int i, index_dying_invader;
    int dx, dy;
    float timer;
    int sfx_i;
    float sfx_timer;

    Horde();
    // std::vector<Invader *> get_alive_invaders();
    void explode_invader(int index);
    Bullet *shoot(float ship_x);
    void update(float delta);
    void draw() const;
};
