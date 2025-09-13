#include "UFO.hpp"
#include <CppInvaders.hpp>

#define Y 40
#define VX 25
#define BORDER 28

#define TIME_DEAD 1.0f
#define TIME_EXPLODING 1.0f
#define TIME_TO_RESPAWN 10.0f

UFO::UFO() {
    state = AWAY;
    timer = 0;
}

bool UFO::collide_rect(Pico_Rect rct, Pico_Anchor anc) const {
    Pico_Rect ufo_rct = {(int)SDL_roundf(x), Y, 16, 8};
    Pico_Anchor ufo_anc = {PICO_CENTER, PICO_TOP};
    return state == ALIVE && pico_rect_vs_rect_ext(rct, anc, ufo_rct, ufo_anc);
}

void UFO::explode() {
    // pico_output_sound("")
    state = EXPLODING;
    timer = 0;
}

void UFO::update(float delta) {
    Pico_Dim size = pico_get_size().log;
    timer += delta;

    switch (state) {
    case AWAY:
        if (timer >= TIME_TO_RESPAWN) {
            bool left = rand() % 2;
            state = ALIVE;
            x = left ? BORDER : (size.x - BORDER);
            vx = left ? VX : -VX;
            timer -= TIME_TO_RESPAWN;
        }
        break;
    case ALIVE:
        x += delta * vx;
        if (BORDER > x || x > size.x - BORDER) {
            state = AWAY;
            timer = 0;
        }
        break;
    case EXPLODING:
        if (timer >= TIME_EXPLODING) {
            state = DEAD;
            score = (rand() % 3 + 1) * 100; // 100, 200 or 300
            timer -= TIME_EXPLODING;
        }
        break;
    case DEAD:
        if (timer >= TIME_DEAD) {
            state = AWAY;
            timer -= TIME_DEAD;
        }
        break;
    }
}

void UFO::draw() const {
    Pico_Pos pos = {(int)SDL_roundf(x), Y};

    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    switch (state) {
    case AWAY:
        break;
    case ALIVE:
        pico_set_crop({0, 0, 24, 8});
        pico_output_draw_image(pos, IMG_UFO);
        break;
    case EXPLODING:
        pico_set_crop({24, 0, 24, 8});
        pico_output_draw_image(pos, IMG_UFO);
        break;
    case DEAD:
        pico_set_color_draw(RED);
        pico_set_crop({0, 0, 0, 0});
        pico_output_draw_fmt(pos, "%d", score);
        break;
    }
}
