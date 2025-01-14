#include "Shot.h"
#include "CppInvaders.h"

void GAMESCOPE::Shot::explode(float time) {
    state = EXPLODING;
    explosion_time = time;
    show_explosion_img = true;
}

void GAMESCOPE::Shot::explode_without_img(float time) {
    state = EXPLODING;
    explosion_time = time;
    show_explosion_img = false;
}

void GAMESCOPE::Shot::draw() {
    int rx = round(x), ry = round(y);
    const char *explosion_img = (vy >= 0 ? IMG_EXP3 : IMG_EXP2);

    switch (state) {
    case ALIVE:
        pico_set_color_draw(WHITE);
        pico_output_draw_rect({ rx, ry, 1, 7 });
        break;
    case EXPLODING:
        if (show_explosion_img) {
            pico_set_image_crop({ 0, 0, 0, 0 });
            pico_output_draw_image({ rx - 3, ry }, explosion_img);
        }
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
        explosion_time -= delta;
        if (explosion_time <= 0) {
            state = DEAD;
        }
        break;
    case DEAD:
        break;
    }
}
