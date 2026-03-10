#include "Splash.hpp"
#include "Play.hpp"
#include "CppInvaders.hpp"
#include "pico.h"
#include <string>

#define TIME_TYPEWRITE 0.05f
#define TIME_WAITING 1.5f

static std::string LINES[] = {
    "PLAY",
    "SPACE INVADERS",
    "*SCORE ADVANCES TABLE*",
    "=? MYSTERY",
    "=30 POINTS",
    "=20 POINTS",
    "=10 POINTS",
    ""};

static Pico_Abs_Pos OFFS[] = {
    {96, 64}, {56,  88}, {24, 120},  {80, 136},
    {80, 152}, {80, 168}, {80, 184}, { 0,  0}};

SplashScene::SplashScene() {
    state = WAITING1;
    timer = 0;
    li = 0;
    ci = 0;
}

SplashScene::~SplashScene() {}

bool SplashScene::typewrite_next_char() {
    if (timer < TIME_TYPEWRITE) {
        return false;
    }

    if (ci++ == LINES[li].length()) {
        li++;
        ci = 0;
    }
    timer -= TIME_TYPEWRITE;
    return true;
}

void SplashScene::process_event(const Pico_Event &event) {
    if (event.type != PICO_EVENT_KEY_DOWN) {
        return;
    }

    if (state == WAITING_KEYPRESS) {
        CppInvaders &game = CppInvaders::get();
        game.scene = new PlayScene;
        game.score = 0;
        delete this;
    } else {
        state = WAITING_KEYPRESS;
        li = 7;
        ci = 0;
    }
}

void SplashScene::update(float delta) {
    timer += delta;

    switch (state) {
    case WAITING1:
        if (timer >= TIME_WAITING) {
            state = TYPEWRITING1;
            timer -= TIME_WAITING;
        }
        break;
    case TYPEWRITING1:
        if (typewrite_next_char() && li == 2) {
            state = WAITING2;
        }
        break;
    case WAITING2:
        if (timer >= TIME_WAITING) {
            state = TYPEWRITING2;
            timer -= TIME_WAITING;
        }
        break;
    case TYPEWRITING2:
        if (typewrite_next_char() && li == 7) {
            state = WAITING_KEYPRESS;
        }
        break;
    case WAITING_KEYPRESS:
        break;
    }
}

void SplashScene::draw() const {
    pico_set_color_draw(PICO_COLOR_WHITE);
    
    for (size_t lj = 0; lj <= (size_t)li; lj++) {
        std::string str = lj == (size_t)li ? LINES[li].substr(0, ci) : LINES[lj];
        if (str.empty()) continue;
        
        Pico_Rel_Rect txt_r = { '!', {(float)OFFS[lj].x, (float)OFFS[lj].y, 0, 8}, PICO_ANCHOR_NW, NULL };
        pico_output_draw_text(str.c_str(), &txt_r);
    }

    if (state == TYPEWRITING2 || state == WAITING_KEYPRESS) {
        // Ensure parent layers exist
        pico_layer_image_mode('=', "ufo", IMG_UFO);
        pico_layer_image_mode('=', "inv1", IMG_INV1);
        pico_layer_image_mode('=', "inv2", IMG_INV2);
        pico_layer_image_mode('=', "inv3", IMG_INV3);

        auto draw_sprite = [](const char* key_par, const char* name, float x, float y, float sw, float sh) {
            char key[256];
            snprintf(key, sizeof(key), "/crop/%s", name);
            Pico_Rel_Rect crop = { '!', {0, 0, sw, sh}, PICO_ANCHOR_NW, NULL };
            pico_layer_sub_mode('=', key, key_par, &crop);
            Pico_Rel_Rect dst = { '!', {x, y, sw, sh}, PICO_ANCHOR_C, NULL };
            pico_output_draw_layer(key, &dst);
        };

        draw_sprite("ufo",  "ufo",  71, 140, 24, 8);
        draw_sprite("inv1", "inv1", 71, 156, 8, 8);
        draw_sprite("inv2", "inv2", 71, 172, 11, 8);
        draw_sprite("inv3", "inv3", 71, 188, 12, 8);
    }
}
