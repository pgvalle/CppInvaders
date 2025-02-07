#include "Screens.h"
#include <string>

#define TYPEWRITING_INTERVAL 0.2f
#define TIME_WAITING 2.5f

static std::string STRING = " GAME OVER";

CppInvaders::Over::Over() {
    state = TYPEWRITING;
    i = 1;
    timer = 0;
}

CppInvaders::Over::~Over() {

}

void CppInvaders::Over::draw() {
    cppinv->game->draw();

    // make the pause menu effect
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 204);
    SDL_Rect r = { 0, 0, 224, 256};
    SDL_RenderFillRect(ren, &r);

    std::string str = STRING.substr(0, i);
    SDL_SetRenderDrawColor(ren, 216, 32, 32, 255);
    fcd_draw_text(72, 64, (char *)str.c_str());
}

void CppInvaders::Over::update(float delta) {
    switch (state) {
    case TYPEWRITING:
        timer += delta;
        if (timer >= TYPEWRITING_INTERVAL) {
            timer = 0;
            if (i++ == (int)STRING.length()) {
                state = WAITING;
                timer = 0;
            }
        }
        break;
    case WAITING:
        timer += delta;
        if (timer >= TIME_WAITING) {
            cppinv->screen = SPLASH;
            cppinv->splash = new Splash;

            delete cppinv->game;
            cppinv->game = nullptr;

            delete cppinv->over;
            cppinv->over = nullptr;
        }
        break;
    }
}

void CppInvaders::Over::process_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_RETURN) {
            if (state == TYPEWRITING) {
                state = WAITING;
                i = STRING.length();
                timer = 0;
            }
            else {
                timer = TIME_WAITING;
            }
        }
        break;
    }
}
