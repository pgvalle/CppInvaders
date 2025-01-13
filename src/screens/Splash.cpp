#include "Splash.h"
#include "Over.h"
#include "Game.h"
#include <string>

#define TYPEWRITE_STEP_DELTA 0.05f
#define TIME_WAITING 1.5f

static std::string LINES[] = {
    " PLAY",
    " SPACE INVADERS",
    " *SCORE ADVANCES TABLE*",
    " =? MYSTERY",
    " =30 POINTS",
    " =20 POINTS",
    " =10 POINTS",
    " "
};

static int LINES_XOFF[] = { 9, 4, 0, 7, 7, 7, 7, 0 };
static int LINES_YOFF[] = { 0, 3, 7, 9, 11, 13, 15, 0 };

CppInvaders::Splash::Splash() {
    state = WAITING1;
    l = 0;
    c = 1;
    time = 0;
}

CppInvaders::Splash::~Splash() {}

bool CppInvaders::Splash::typewrite_next_char() {
    if (time >= TYPEWRITE_STEP_DELTA) {
        if (c++ == LINES[l].length()) {
            l++;
            c = 1;
        }
        time = 0;
        return true;
    }
    return false;
}

void CppInvaders::Splash::draw() {
    pico_set_color_clear({ 0x0, 0x0, 0x0, 0x0 });
    pico_output_clear();

    cppinv->draw_counters();

    pico_set_color_draw(WHITE);
    for (int k = 0; k <= l; k++) {
        std::string str = (k < l ? LINES[k] : LINES[l].substr(0, c));
        int xoff = 16 + 8 * LINES_XOFF[k];
        int yoff = 64 + 8 * LINES_YOFF[k];
        pico_output_draw_text({ xoff, yoff }, (char *)str.c_str());
    }

    if (l > 2) {
        pico_set_image_crop({ 0, 0, 24, 8 });
        pico_output_draw_image({ 59, 136 }, IMG_UFO);
        pico_set_image_crop({ 0, 0, 8, 8 });
        pico_output_draw_image({ 67, 152 }, IMG_INV1);
        pico_set_image_crop({0, 0, 11, 8});
        pico_output_draw_image({ 66, 168 }, IMG_INV2);
        pico_set_image_crop({0, 0, 12, 8});
        pico_output_draw_image({ 65, 184 }, IMG_INV3);
    }
}

void CppInvaders::Splash::update(float delta) {
    time += delta;

    switch (state) {
    case WAITING1:
        if (time >= TIME_WAITING) {
            state = TYPEWRITING1;
            time = 0;
        }
        break;
    case TYPEWRITING1:
        if (typewrite_next_char() && l == 2) {
            state = WAITING2;
        }
        break;
    case WAITING2:
        if (time >= TIME_WAITING) {
            state = TYPEWRITING2;
            l = 3;
            c = 1;
            time = 0;
        }
        break;
    case TYPEWRITING2:
        if (typewrite_next_char() && l == 7) {
            state = WAITING_KEYPRESS;
        }
        break;
    case WAITING_KEYPRESS:
        break;
    }
}

void CppInvaders::Splash::process_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym != SDLK_RETURN) {
            break;
        }

        if (state == WAITING_KEYPRESS) {
            cppinv->score = 0;

            delete this;
            cppinv->splash = nullptr;

            cppinv->screen = GAME;
            cppinv->game = new Game;
        }
        else {
            state = WAITING_KEYPRESS;
            l = 7;
            c = 1;
        }

        break;
    }
}
