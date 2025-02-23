#include "UFO.h"
#include "screens/Game.h"

#define Y 40
#define VX 25
#define LEFT_LIMIT 8
#define RIGHT_LIMIT 192

#define TIME_DEAD 1
#define TIME_EXPLODING 1
#define TIME_TO_RESPAWN 10.0f

void UFO::explode() {
    pico_assert(state == ALIVE);
    state = EXPLODING;
    timer = 0;
}

void UFO::draw() {
    static char text[4];
    int rx = (int)round(x);

    switch (state) {
    case AWAY:
        break;
    case ALIVE:
        pico_set_image_crop({ 0, 0, 24, 8 });
        pico_output_draw_image({ rx, Y }, IMG_UFO);
        break;
    case EXPLODING:
        pico_set_image_crop({ 24, 0, 24, 8 });
        pico_output_draw_image({ rx, Y }, IMG_UFO);
        break;
    case DEAD:
        sprintf(text, "%d", score);
        pico_set_color_draw(RED);
        pico_output_draw_text({ rx, Y }, text);
        break;
    }
}

void UFO::update(float delta) {
    switch (state) {
    case AWAY:
        timer += delta;
        if (timer >= TIME_TO_RESPAWN) {
            state = ALIVE;
            x = (rand() % 2 ? LEFT_LIMIT : RIGHT_LIMIT); // choose a corner
            vx = (x == LEFT_LIMIT ? VX : -VX);           // set correct velocity
        }
        break;
    case ALIVE:
        x += delta * vx;
        if (LEFT_LIMIT > x || x > RIGHT_LIMIT) {
            state = AWAY;
            timer = 0;
        }
        break;
    case EXPLODING:
        timer += delta;
        if (timer >= TIME_EXPLODING) {
            state = DEAD;
            timer = 0;
            score = (rand() % 3 + 1) * 100; // 100, 200 or 300
            cppinv->add_to_score(score);
        }
        break;
    case DEAD:
        timer += delta;
        if (timer > TIME_DEAD) {
            state = AWAY;
            timer = 0;
        }
        break;
    }
}
