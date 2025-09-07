#include "Bullet.hpp"
#include "CppInvaders.hpp"

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
    die();
    show_explosion = true;
}

void Bullet::explode_without_img(float time) {
    explode(time);
    show_explosion = false;
}

void Bullet::draw() const {
    Pico_Pos pos = {(int)round(x), (int)round(y)};
    const char *explosion_img = vy < 0 ? IMG_EXP2 : IMG_EXP3;

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

void Bullet::update(float delta) {
    switch (state) {
    case ALIVE:
        y += delta * vy;
        if (y < 33 || y >= 232) { // TODO: ick
            y = (y < 33 ? 33 : 232);
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
