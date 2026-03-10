#include "Bullet.hpp"
#include <CppInvaders.hpp>

#define Y_MIN 38
#define Y_MAX 232

Bullet::Bullet(float x, float y, float vy) {
    state = ALIVE;
    this->x = x;
    this->y = y;
    this->vy = vy;
    show_explosion = true;
}

Pico_Abs_Rect Bullet::get_rect() const {
    return {(int)round(x), (int)round(y), 1, 7};
}

void Bullet::die(float time) {
    state = EXPLODING;
    timer = time;
    show_explosion = false;
}

void Bullet::explode(float time) {
    die(time);
    show_explosion = true;
}

void Bullet::update(float delta) {
    switch (state) {
    case ALIVE:
        y += delta * vy;
        if (y <= Y_MIN || y >= Y_MAX) {
            y = y <= Y_MIN ? Y_MIN : Y_MAX;
            explode(0.3);
        }
        break;
    case EXPLODING:
        timer -= delta;
        if (timer <= 0) {
            state = DEAD;
        }
        break;
    case DEAD:
        break;
    }
}

void Bullet::draw() const {
    float draw_x = (float)round(x);
    float draw_y = (float)round(y);
    const char *explosion_img = vy < 0 ? IMG_EXP2 : IMG_EXP3;

    switch (state) {
    case ALIVE:
        {
            pico_set_color_draw(PICO_COLOR_WHITE);
            Pico_Rel_Rect r = { '!', {draw_x, draw_y, 1, 7}, PICO_ANCHOR_C, NULL };
            pico_output_draw_rect(&r);
        }
        break;
    case EXPLODING:
        if (show_explosion) {
            Pico_Rel_Rect r = { '!', {draw_x, draw_y, 0, 0}, PICO_ANCHOR_C, NULL };
            pico_output_draw_image(explosion_img, &r);
        }
        break;
    case DEAD:
        break;
    }
}
