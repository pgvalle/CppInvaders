#pragma once

#include "Shot.h"

struct Shield {
    int x;
    bool bits[352];

    void deploy(int x);
    bool damage(Shot *shot);

    void draw();
    // bool collidedWithShot(Shot *shot);
    // void collideWithHorde(const Horde &horde);
};