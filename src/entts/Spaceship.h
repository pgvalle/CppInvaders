#pragma once

#include "screens/Game.h"

struct GAMESCOPE::Spaceship {
    enum State {
        DEPLOYING, DEPLOYED, EXPLODING
    };

    State state = DEPLOYING;
    float x, time = 0;
    int lives = 3, explosion_frames;

    void explode();
    
    void draw();
    void update(float delta);
    //Shot *shoot();
    //bool collidedWithShot(Shot *shot);
};
