#pragma once

#include "Invader.hpp"

struct Horde {
    enum State {
        PREDEPLOY, DEPLOYING, MARCHING, FROZEN
    } state;
    Invader invaders[55];
    int i, index_dying_invader;
    int dx, dy;
    float timer;

    Horde();
    // std::vector<Invader *> get_alive_invaders();
    void explode_invader(int index);
    // Shot *shoot(float spaceship_x);
    void update(float delta);
    void draw() const;
};
