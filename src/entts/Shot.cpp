#include "Shot.h"
#include "CppInvaders.h"

void GAMESCOPE::Shot::draw() {
    int rx = round(x), ry = round(y);
    char *explosion_img = (char *)(vy >= 0 ? IMG_EXP3 : IMG_EXP2);

    switch (state) {
    case ALIVE:
        pico_set_color_draw(WHITE);
        pico_output_draw_rect({ rx, ry, 1, 7 });
        break;
    case EXPLODING:
        pico_set_image_crop({ 0, 0, 0, 0 });
        pico_output_draw_image({ rx - 3, ry }, explosion_img);
        break;
    case DEAD:
        break;
    }
}

void GAMESCOPE::Shot::update(float delta) {
    switch (state) {
    case ALIVE:
        y += delta * vy;
        if (y < 36 || y >  232) {
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
