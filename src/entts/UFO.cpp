#include "UFO.h"
#include "CppInvaders.h"

#define Y 40
#define VX 25
#define LEFT_LIMIT 8
#define RIGHT_LIMIT 192

#define TIME_DEAD 1.5f
#define TIME_EXPLODING 1.5f
#define TIME_TO_RESPAWN 10.0f

void GAMESCOPE::UFO::explode() {
    if (state == ALIVE) {
        state = EXPLODING;
        time = 0;
    }
}

void GAMESCOPE::UFO::draw() {
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

void GAMESCOPE::UFO::update(float delta) {
    switch (state) {
    case AWAY:
        time += delta;
        if (time >= TIME_TO_RESPAWN) {
            state = ALIVE;
            x = (rand() % 2 ? LEFT_LIMIT : RIGHT_LIMIT); // choose a corner
            vx = (x == LEFT_LIMIT ? VX : -VX);           // set correct velocity
        }
        break;
    case ALIVE:
        x += delta * vx;
        if (LEFT_LIMIT > x || x > RIGHT_LIMIT) {
            state = AWAY;
            time = 0;
        }
        break;
    case EXPLODING:
        time += delta;
        if (time >= TIME_EXPLODING) {
            state = DEAD;
            time = 0;
            score = (rand() % 3 + 1) * 100; // 100, 200 or 300
            cppinv->add_to_score(score);
        }
        break;
    case DEAD:
        time += delta;
        if (time > TIME_DEAD) {
            state = AWAY;
            time = 0;
        }
        break;
    }
}

// bool GAMESCOPE::UFO::collidedWithShot(const Shot *shot) {
//     if (state != ALIVE) {
//         return false;
//     }

//     SDL_Rect ufoRect = {(int)round(x + 4), Y, 16, 8},
//              shotRect = shot->getRect();

//     if (SDL_HasIntersection(&ufoRect, &shotRect)) {
//         state = EXPLODING;
//         time = 0;
//         return true;
//     }

//     return false;
// }