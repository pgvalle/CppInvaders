#pragma once

struct Bullet;

struct Spaceship {
    enum State {
        DEPLOYING, DEPLOYED, EXPLODING
    } state;
    float x;
    float timer;
    int explosion_frames;

    Spaceship();
    void explode();
    Bullet* shoot();
    void update(float delta);
    void draw() const;
};
