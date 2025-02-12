#include "Shot.h"
#include "screens/Game.h"

void Shot::explode(float time) {
    assert(state == ALIVE);

    state = EXPLODING;
    explosion_timer = time;
    show_explosion_img = true;
}

void Shot::explode_without_img(float time) {
    explode(time);
    show_explosion_img = false;
}

void Shot::draw() {
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

void Shot::update(float delta) {
    switch (state) {
    case ALIVE:
        y += delta * vy;
        if (y < 33 || y >= 232) {
            y = (y < 33 ? 33 : 232);
            explode(0.3);
        }
        break;
    case EXPLODING:
        explosion_timer -= delta;
        if (explosion_timer <= 0) {
            state = DEAD;
        }
        break;
    case DEAD:
        break;
    }
}
