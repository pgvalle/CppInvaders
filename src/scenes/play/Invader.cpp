#include "Invader.hpp"
#include <CppInvaders.hpp>

int Invader::counter = 0;

Invader::Invader() {
    int row = 4 - counter / 11;
    int col = counter % 11;

    state = DEAD;
    type = row / 2;
    x = 32 + 16 * col;
    y = 64 + 16 * row;
    counter++;
}

Pico_Rect Invader::get_rect() const {
    switch (type) {
    case 0:
        return {x, y, 8, 8};
    case 1:
        return {x, y, 11, 8};
    default:
        return {x, y, 12, 8};
    }
}

const char* Invader::get_image() const {
    switch (type) {
    case 0:
        return IMG_INV1;
    case 1:
        return IMG_INV2;
    default:
        return IMG_INV3;
    }
}

bool Invader::is_alive() const {
    return state == UP || state == DOWN;
}

void Invader::move(int dx, int dy) {
    if (state == DOWN) {
        state = UP;
    } else if (state == UP) {
        state = DOWN;
    }

    x += dx;
    y += dy;
}

void Invader::draw() const {
    const char* img = get_image();
    Pico_Rect rect = get_rect();
    Pico_Pos pos = {x, y};

    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    switch (state) {
    case UP:
        pico_set_crop({rect.w, 0, rect.w, rect.h});
        pico_output_draw_image(pos, img);
        break;
    case DOWN:
        pico_set_crop({0, 0, rect.w, rect.h});
        pico_output_draw_image(pos, img);
        break;
    case DYING:
        pico_set_crop({0, 0, 0, 0});
        pico_output_draw_image(pos, IMG_EXP1);
        break;
    case DEAD:
        break;
    }
}
