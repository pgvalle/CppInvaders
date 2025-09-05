#include "UFO.hpp"
#include "CppInvaders.hpp"

#define Y 40
#define VX 25
#define BORDER 28

#define TIME_DEAD 1
#define TIME_EXPLODING 1
#define TIME_TO_RESPAWN 10.0f

UFO::UFO() {
    state = AWAY;
    timer = 0;
}

void UFO::explode() {
    state = EXPLODING;
    timer = 0;
}

void UFO::update(float delta) {
    Pico_Dim size = pico_get_size().log;
    timer += delta;

    switch (state) {
    case AWAY:
        if (timer >= TIME_TO_RESPAWN) {
            state = ALIVE;
            x = BORDER;
            vx = VX;
            if (rand() % 2) {
                x = size.x - BORDER;
                vx = -VX;
            }
        }
        break;
    case ALIVE:
        x += delta * vx;
        if (BORDER > x || x > size.x - BORDER) {
            state = AWAY;
            timer = 0;
        }
        break;
    case EXPLODING:
        if (timer >= TIME_EXPLODING) {
            state = DEAD;
            timer = 0;
            score = (rand() % 3 + 1) * 100; // 100, 200 or 300
            //cppinv->add_to_score(score);
        }
        break;
    case DEAD:
        if (timer > TIME_DEAD) {
            state = AWAY;
            timer = 0;
        }
        break;
    }
}

void UFO::draw() const {
    static char text[16];
    Pico_Pos pos = {(int)round(x), Y};

    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    switch (state) {
    case AWAY:
        break;
    case ALIVE:
        pico_set_crop({0, 0, 24, 8});
        pico_output_draw_image(pos, IMG_UFO);
        break;
    case EXPLODING:
        pico_set_crop({24, 0, 24, 8});
        pico_output_draw_image(pos, IMG_UFO);
        break;
    case DEAD:
        sprintf(text, "%d", score);
        pico_set_color_draw(RED);
        pico_output_draw_text(pos, text);
        break;
    }
}
