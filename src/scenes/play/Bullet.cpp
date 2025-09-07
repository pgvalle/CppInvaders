#include "Bullet.hpp"
#include <CppInvaders.hpp>

#define UP_DEAD_END 38
#define DOWN_DEAD_END 232

Bullet::Bullet(float x, float y, float vy) {
    state = ALIVE;
    this->x = x;
    this->y = y;
    this->vy = vy;
    show_explosion = true;
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
        if (y < UP_DEAD_END || y >= DOWN_DEAD_END) {
            y = y < UP_DEAD_END ? UP_DEAD_END : DOWN_DEAD_END;
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
    Pico_Pos pos = {(int)round(x), (int)round(y)};
    const char *explosion_img = vy < 0 ? IMG_EXP2 : IMG_EXP3;

    pico_set_crop({0, 0, 0, 0});
    pico_set_anchor_draw({PICO_CENTER, PICO_MIDDLE});
    switch (state) {
    case ALIVE:
        pico_set_color_draw(WHITE);
        pico_output_draw_rect({pos.x, pos.y, 1, 7});
        break;
    case EXPLODING:
        if (show_explosion) {
            pico_output_draw_image(pos, explosion_img);
        }
        break;
    case DEAD:
        break;
    }
}
