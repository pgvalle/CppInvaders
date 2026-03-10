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

bool UFO::collide_rect(Pico_Abs_Rect rct, Pico_Anchor anc) const {
    Pico_Rel_Rect ufo_rct = { '!', {(float)SDL_roundf(x), (float)Y, 16, 8}, PICO_ANCHOR_N, NULL };
    Pico_Rel_Rect other_rct = { '!', {(float)rct.x, (float)rct.y, (float)rct.w, (float)rct.h}, anc, NULL };
    return state == ALIVE && pico_vs_rect_rect(&ufo_rct, &other_rct);
}

void UFO::explode() {
    // pico_output_sound("")
    state = EXPLODING;
    timer = 0;
}

void UFO::update(float delta) {
    Pico_Abs_Dim size;
    pico_get_view(NULL, &size, NULL, NULL, NULL, NULL, NULL, NULL);
    timer += delta;

    switch (state) {
    case AWAY:
        if (timer >= TIME_TO_RESPAWN) {
            bool left = rand() % 2;
            state = ALIVE;
            x = left ? BORDER : (size.w - BORDER);
            vx = left ? VX : -VX;
            timer -= TIME_TO_RESPAWN;
        }
        break;
    case ALIVE:
        x += delta * vx;
        if (BORDER > x || x > size.w - BORDER) {
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
    if (state == AWAY) return;

    float draw_x = (float)SDL_roundf(x);
    float draw_y = (float)Y;
    
    char key[256];
    Pico_Rel_Rect crop = { '!', {0, 0, 24, 8}, PICO_ANCHOR_NW, NULL };

    pico_layer_image_mode('=', "ufo", IMG_UFO);

    switch (state) {
    case ALIVE:
        snprintf(key, sizeof(key), "/crop/ufo/alive");
        crop.x = 0;
        pico_layer_sub_mode('=', key, "ufo", &crop);
        {
            Pico_Rel_Rect dst = { '!', {draw_x, draw_y, 24, 8}, PICO_ANCHOR_N, NULL };
            pico_output_draw_layer(key, &dst);
        }
        break;
    case EXPLODING:
        snprintf(key, sizeof(key), "/crop/ufo/exploding");
        crop.x = 24.0f;
        pico_layer_sub_mode('=', key, "ufo", &crop);
        {
            Pico_Rel_Rect dst = { '!', {draw_x, draw_y, 24, 8}, PICO_ANCHOR_N, NULL };
            pico_output_draw_layer(key, &dst);
        }
        break;
    case DEAD:
        {
            pico_set_color_draw(PICO_COLOR_RED);
            char score_buf[16];
            snprintf(score_buf, sizeof(score_buf), "%d", score);
            Pico_Rel_Rect txt_r = { '!', {draw_x, draw_y, 0, 8}, PICO_ANCHOR_N, NULL };
            pico_output_draw_text(score_buf, &txt_r);
        }
        break;
    default:
        break;
    }
}
