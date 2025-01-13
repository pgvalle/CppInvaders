#include "Shot.h"
#include "CppInvaders.h"

void Shot::draw() {
    int rx = round(x), ry = round(y);
    const char *explosion_img = (vy >= 0 ? IMG_EXP3 : IMG_EXP2);
    switch (state) {
    case ALIVE:
        pico_set_color_draw(WHITE);
        pico_output_draw_rect({ rx, ry, 1, 7 });
        break;
    case EXPLODING:
        pico_set_image_crop({ 0, 0, 0, 0 });
        pico_output_draw_image({ rx - 3, ry }, (char *)explosion_img);
        break;
    case DEAD:
        break;
    }
}

void Shot::update(float delta) {
    switch (state) {
    case ALIVE:
        y += delta * vy;
        if (y <= 16 || y >=  250) {
            state = EXPLODING;
        }
        break;
    case EXPLODING:
        explosion_time += delta;
        if (explosion_time >= 0.3f) {
            state = DEAD;
        }
        break;
    case DEAD:
        break;
    }
}
