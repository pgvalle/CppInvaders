#include "Horde.h"
#include "screens/Game.h"

SDL_Rect Invader::get_rect() {
    switch (type) {
    case 0:
        return { x + 2, y, 8, 8 };
    case 1:
        return { x + 1, y, 11, 8 };
    case 2:
    default:
        return { x, y, 12, 8 };
    }
}

void Invader::deploy(int i) {
    int row = 4 - i / 11, col = i % 11;

    state = DOWN;
    type = row / 2;
    x = 26 + 16 * col;
    y = 64 + 16 * row;
}

void Invader::move(int dx, int dy) {
    pico_assert(state != DEAD);
 
    state = (state == UP ? DOWN : UP);
    x += dx;
    y += dy;
}

void Invader::draw() {
    if (state == DEAD) {
        return;
    }

    SDL_Rect crop = get_rect();
    SDL_Point pos = { crop.x, crop.y };

    crop.x = (state == DOWN ? 0 : crop.w);
    crop.y = 0;
    pico_set_image_crop(crop);

    switch (type) {
    case 0:
        pico_output_draw_image(pos, IMG_INV1);
        break;
    case 1:
        pico_output_draw_image(pos, IMG_INV2);
        break;
    case 2:
        pico_output_draw_image(pos, IMG_INV3);
        break;
    }
}