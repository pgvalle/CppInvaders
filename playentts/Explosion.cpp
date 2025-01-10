#include "Explosion.h"

void Explosion::draw() {
    pico_set_image_crop(crop);
    pico_output_draw_image({ x, y }, (char *)img.c_str());
}

void Explosion::update(float delta) {
    if (lifetime > 0) {
        lifetime -= delta;
    }
}