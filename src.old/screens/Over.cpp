#include "Screens.h"
#include <string>

#define TYPEWRITING_INTERVAL 0.2f
#define TIME_WAITING 2.5f

extern SDL_Window *WIN;
static SDL_BlendMode blendBackup;

static std::string STRING = " GAME OVER";

CppInvaders::Over::Over() {
    state = TYPEWRITING;
    i = 1;
    timer = 0;

    SDL_GetRenderDrawBlendMode(SDL_GetRenderer(WIN),  &blendBackup);
    SDL_SetRenderDrawBlendMode(SDL_GetRenderer(WIN), SDL_BLENDMODE_BLEND);
}

CppInvaders::Over::~Over() {
    SDL_SetRenderDrawBlendMode(SDL_GetRenderer(WIN), blendBackup);
}

void CppInvaders::Over::draw() {
    cppinv->game->draw();

    // make the pause menu effect
    pico_set_color_draw({ 0, 0, 0, 204 });
    pico_output_draw_rect({ 0, 0, 224, 256 });

    std::string str = STRING.substr(0, i);
    pico_set_color_draw(RED);
    pico_output_draw_text({ 72, 64 }, (char *)str.c_str());
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
