#include "Screens.h"

#define PAUSE_SYMBOL_BLINK 0.3
#define TIME_TO_RESUME 3

CppInvaders::Pause::Pause() {
    state = PAUSED;
    timer = 0;
    pause_symbol = true;
}

CppInvaders::Pause::~Pause() {
    
}

void CppInvaders::Pause::draw() {
    static char text[16];

    cppinv->game->draw();

    // make the pause menu effect
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 204);
    SDL_Rect r = {0, 0, 224, 256};
    SDL_RenderFillRect(ren, &r);

    cppinv->draw_credit_counter();

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    switch (state) {
    case RESUMING:
        sprintf(text, "%02d", 3 - (int)timer);
        fcd_draw_text(104, 8, text);
        break;
    case PAUSED:
        if (pause_symbol) {
            fcd_draw_text(104, 8, "||");
        }
        break;
    }
}

void CppInvaders::Pause::update(float delta) {
    switch (state) {
    case PAUSED:
        timer += delta;
        if (timer >= PAUSE_SYMBOL_BLINK) {
            timer = 0;
            pause_symbol = !pause_symbol;
        }
        break;
    case RESUMING:
        timer += delta;
        if (timer >= TIME_TO_RESUME) {
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
            timer = 0;
            pause_symbol = true;
            break;
        }
        break;
    }
}
