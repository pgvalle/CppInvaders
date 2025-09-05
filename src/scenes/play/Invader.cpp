#include "Invader.hpp"
#include "CppInvaders.hpp"

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

void Invader::move(int dx, int dy) {
    if (state == DEAD) {
        return;
    }

    state = (state == UP ? DOWN : UP);
    x += dx;
    y += dy;
}

void Invader::draw() const {
    if (state == DEAD) {
        return;
    }

    Pico_Rect rect = get_rect();
    Pico_Pos pos = {rect.x, rect.y};

    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    pico_set_crop({(state == UP ? rect.w : 0), 0, rect.w, rect.h});
    
    switch (type) {
    case 0:
        pico_output_draw_image(pos, IMG_INV1);
        break;
    case 1:
        pico_output_draw_image(pos, IMG_INV2);
        break;
    default:
        pico_output_draw_image(pos, IMG_INV3);
        break;
    }
}
