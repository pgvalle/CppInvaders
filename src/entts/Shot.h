#pragma once

struct Shot {
    enum State {
        ALIVE, EXPLODING, DEAD
    };

    State state = DEAD;
    float x, y, vy;
    float explosion_time = 0;
    bool show_explosion_img;

    void explode(float time);
    void explode_without_img(float time);

    void draw();
    void update(float delta);
};
