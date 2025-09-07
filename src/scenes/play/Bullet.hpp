#pragma once

struct Bullet {
    enum State {
        ALIVE, EXPLODING, DEAD
    } state = DEAD;
    float x, y, vy;
    float timer;
    bool show_explosion;

    Bullet(float x, float y, float vy);
    void die(float time);
    void explode(float time);

    void update(float delta);
    void draw() const;
};
