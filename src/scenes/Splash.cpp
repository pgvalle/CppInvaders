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

static Pico_Pos OFFS[] = {
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
    if (event.type != SDL_KEYDOWN || event.key.keysym.sym != SDLK_RETURN) {
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
    pico_set_color_draw(WHITE);
    pico_set_anchor_draw({PICO_LEFT, PICO_TOP});
    for (size_t lj = 0; lj <= li; lj++) {
        std::string str = lj == li ? LINES[li].substr(0, ci) : LINES[lj];
        pico_output_draw_text(OFFS[lj], str.c_str());
    }

    if (state == TYPEWRITING2 || state == WAITING_KEYPRESS) {
        pico_set_anchor_draw({PICO_CENTER, PICO_MIDDLE});
        pico_set_crop({0, 0, 24, 8});
        pico_output_draw_image({71, 140}, IMG_UFO);
        pico_set_crop({0, 0, 8, 8});
        pico_output_draw_image({71, 156}, IMG_INV1);
        pico_set_crop({0, 0, 11, 8});
        pico_output_draw_image({71, 172}, IMG_INV2);
        pico_set_crop({0, 0, 12, 8});
        pico_output_draw_image({71, 188}, IMG_INV3);
    }
}
