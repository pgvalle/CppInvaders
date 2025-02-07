#include "Screens.h"
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

static int X_OFFS[] = { 72, 32, 0, 56, 56, 56, 56, 0 };
static int Y_OFFS[] = { 0, 24, 56, 72, 88, 104, 120, 0 };

CppInvaders::Splash::Splash() {
    state = WAITING1;
    l = 0;
    c = 1;
    timer = 0;
}

CppInvaders::Splash::~Splash() {}

bool CppInvaders::Splash::typewrite_next_char() {
    if (timer >= TYPEWRITE_STEP_DELTA) {
        if (c++ == (int)LINES[l].length()) {
            l++;
            c = 1;
        }
        timer = 0;
        return true;
    }
    return false;
}

void CppInvaders::Splash::draw() {
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);

    cppinv->draw_scoreboard();
    cppinv->draw_credit_counter();

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    for (int k = 0; k <= l; k++) {
        std::string str = (k < l ? LINES[k] : LINES[l].substr(0, c));
        SDL_Point pos = { 16 + X_OFFS[k], 64 + Y_OFFS[k] };
        fcd_draw_text(pos.x, pos.y, (char *)str.c_str());
    }

    if (l > 2) {
        fcd_draw_image(59, 136, IMG_UFO, { 0, 0, 24, 8 });
        fcd_draw_image(67, 152, IMG_INV1, { 0, 0, 8, 8 });
        fcd_draw_image(66, 168, IMG_INV2, {0, 0, 11, 8});
        fcd_draw_image(65, 184, IMG_INV3, {0, 0, 12, 8});
    }
}

void CppInvaders::Splash::update(float delta) {
    switch (state) {
    case WAITING1:
        timer += delta;
        if (timer >= TIME_WAITING) {
            state = TYPEWRITING1;
            timer = 0;
        }
        break;
    case TYPEWRITING1:
        timer += delta;
        if (typewrite_next_char() && l == 2) {
            state = WAITING2;
        }
        break;
    case WAITING2:
        timer += delta;
        if (timer >= TIME_WAITING) {
            state = TYPEWRITING2;
            l = 3;
            c = 1;
            timer = 0;
        }
        break;
    case TYPEWRITING2:
        timer += delta;
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
            cppinv->screen = GAME;
            cppinv->game = new Game;

            delete cppinv->splash;
            cppinv->splash = nullptr;

            cppinv->score = 0;
        }
        else {
            state = WAITING_KEYPRESS;
            l = 7;
            c = 1;
        }

        break;
    }
}
