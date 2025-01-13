#include "Over.h"
#include "Splash.h"
#include <string>

#define TYPEWRITING_INTERVAL 0.1f
#define TIME_WAITING 2.5f

extern SDL_Window *WIN;
static SDL_BlendMode blendBackup;

static std::string STRING = " GAME OVER";

CppInvaders::Over::Over() {
    state = TYPEWRITING;
    i = 1;
    time = 0;

    SDL_GetRenderDrawBlendMode(SDL_GetRenderer(WIN),  &blendBackup);
    SDL_SetRenderDrawBlendMode(SDL_GetRenderer(WIN), SDL_BLENDMODE_BLEND);
}

CppInvaders::Over::~Over() {
    SDL_SetRenderDrawBlendMode(SDL_GetRenderer(WIN), blendBackup);
}

void CppInvaders::Over::draw() {
    cppinv->splash->draw();

    // make the pause menu effect
    pico_set_color_draw({ 0, 0, 0, 204 });
    pico_output_draw_rect({ 0, 0, 224, 256 });

    std::string str = STRING.substr(0, i);
    pico_set_color_draw(RED);
    pico_output_draw_text({ 72, 64 }, (char *)str.c_str());
}

void CppInvaders::Over::update(float delta) {
    time += delta;

    switch (state) {
    case TYPEWRITING:
        if (time >= TYPEWRITING_INTERVAL) {
            time = 0;
            if (i++ == STRING.length()) {
                state = WAITING;
                time = 0;
            }
        }
        break;

    case WAITING:
        if (time >= TIME_WAITING) {
            delete cppinv->game;
            cppinv->game = nullptr;

            delete this;
            cppinv->over = nullptr;

            cppinv->screen = SPLASH;
            delete cppinv->splash;
            cppinv->splash = new Splash;
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
                time = 0;
            }
            else {
                time = TIME_WAITING;
            }
        }
        break;
    }
}
