#pragma once

#include "Shot.h"

struct Spaceship {
    enum State {
        DEPLOYING, DEPLOYED, EXPLODING
    };

    State state = DEPLOYING;
    float x, time = 0;
    int lives = 3, explosion_frames;

    void explode();
    Shot *shoot();
    
    void draw();
    void update(float delta);
    //bool collidedWithShot(Shot *shot);
};
