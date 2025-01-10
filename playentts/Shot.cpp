#include "Shot.h"

void Shot::draw() {
    pico_set_color_draw({ 255, 255, 255, 255 });
    pico_output_draw_rect({ (int)round(x), (int)round(y), 1, 7 });
}

void Shot::update(float delta) {
    y += delta * vy;
}
