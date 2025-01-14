#include "Screens.h"

#define PAUSE_SYMBOL_BLINK 0.5
#define TIME_TO_RESUME 3

extern SDL_Window *WIN;
static SDL_BlendMode blendBackup;

CppInvaders::Pause::Pause() {
    state = PAUSED;
    time = 0;
    pause_symbol = true;

    SDL_GetRenderDrawBlendMode(SDL_GetRenderer(WIN),  &blendBackup);
    SDL_SetRenderDrawBlendMode(SDL_GetRenderer(WIN), SDL_BLENDMODE_BLEND);
}

CppInvaders::Pause::~Pause() {
    SDL_SetRenderDrawBlendMode(SDL_GetRenderer(WIN), blendBackup);
}

void CppInvaders::Pause::draw() {
    static char text[16];

    cppinv->game->draw();

    // make the pause menu effect
    pico_set_style(PICO_FILL);
    pico_set_color_draw({ 0, 0, 0, 204 });
    pico_output_draw_rect({ 0, 0, 224, 256 });

    cppinv->draw_credit_counter();

    pico_set_color_draw(WHITE);
    switch (state) {
    case RESUMING:
        sprintf(text, "%02d", 3 - (int)time);
        pico_output_draw_text({ 104, 8 }, text);
        break;
    case PAUSED:
        if (pause_symbol) {
            pico_output_draw_text({ 104, 8 }, "||");
        }
        break;
    }
}

void CppInvaders::Pause::update(float delta) {
    time += delta;

    switch (state) {
    case PAUSED:
        if (time >= PAUSE_SYMBOL_BLINK) {
            time = 0;
            pause_symbol = !pause_symbol;
        }
        break;
    case RESUMING:
        if (time >= TIME_TO_RESUME) {
            cppinv->screen = GAME;
            delete cppinv->pause;
            cppinv->pause = nullptr;
        }
        break;
    }
}

void CppInvaders::Pause::process_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_r: // reset game
            cppinv->screen = SPLASH;
            cppinv->splash = new Splash;

            delete cppinv->pause;
            cppinv->pause = nullptr;
            break;
        case SDLK_ESCAPE: // start resume countdown or cancel it
            state = (state == PAUSED ? RESUMING : PAUSED);
            time = 0;
            pause_symbol = true;
            break;
        }
        break;
    }
}
