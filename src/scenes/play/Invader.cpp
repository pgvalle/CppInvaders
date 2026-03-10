#include "Invader.hpp"
#include <CppInvaders.hpp>

#define TIME_DYING 0.5f

int Invader::dx = 2, Invader::dy = 0;

Invader::Invader(int i) {
    int row = 4 - i / 11;
    int col = i % 11;

    state = DOWN;
    type = row / 2;
    x = 32 + 16 * col;
    y = 64 + 16 * row;
}

Pico_Abs_Rect Invader::get_rect() const {
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

void Invader::kill() {
    state = DYING;
    timer = 0;
}

void Invader::update(float delta) {
    switch (state) {
    case DOWN:
        state = UP;
        x += dx;
        y += dy;
        break;
    case UP:
        state = DOWN;
        x += dx;
        y += dy;
        break;
    case DYING:
        timer += delta;
        if (timer >= TIME_DYING) {
            state = DEAD;
        }
        break;
    case DEAD:
        break;
    }

}

void Invader::draw() const {
    if (state == DEAD) return;

    const char* img = get_image();
    Pico_Abs_Rect r = get_rect();
    float draw_x = (float)x;
    float draw_y = (float)y;
    float w = (float)r.w;
    float h = (float)r.h;

    char key[256];
    Pico_Rel_Rect crop = { '!', {0, 0, w, h}, PICO_ANCHOR_NW, NULL };

    switch (state) {
    case UP:
        snprintf(key, sizeof(key), "/crop/invader/%d/up", type);
        crop.x = w;
        pico_layer_image_mode('=', img, img);
        pico_layer_sub_mode('=', key, img, &crop);
        break;
    case DOWN:
        snprintf(key, sizeof(key), "/crop/invader/%d/down", type);
        crop.x = 0;
        pico_layer_image_mode('=', img, img);
        pico_layer_sub_mode('=', key, img, &crop);
        break;
    case DYING:
        snprintf(key, sizeof(key), "/crop/invader/dying");
        img = IMG_EXP1;
        // Explosion frame 0
        w = 16.0f; h = 8.0f; // Assumed explosion size
        crop.w = w; crop.h = h;
        pico_layer_image_mode('=', img, img);
        pico_layer_sub_mode('=', key, img, &crop);
        break;
    default:
        return;
    }

    Pico_Rel_Rect dst = { '!', {draw_x, draw_y, w, h}, PICO_ANCHOR_N, NULL };
    pico_output_draw_layer(key, &dst);
}
