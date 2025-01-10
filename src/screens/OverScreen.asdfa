#include "OverScreen.h"
#include "SplashScreen.h"

#include <string>

#define TYPEWRITING_INTERVAL 0.1f
#define TIME_WAITING 2.5f

static std::string STRING = " GAME OVER";

OverScreen::OverScreen() {
    state = TYPEWRITING;
    i = 1;
    time = 0;
}

OverScreen::~OverScreen() {
    delete prev_screen;
}

void OverScreen::draw()
{
    prev_screen->draw();

    // make the pause menu effect
    pico_set_color_draw({ 0, 0, 0, 204 });
    pico_output_draw_rect({ 0, 0, 224, 256 });

    std::string str = STRING.substr(0, i);
    pico_output_draw_text({ 80, 64 }, (char *)str.c_str());
}

void OverScreen::update(float delta) {
    time += delta;

    switch (state) {
    case TYPEWRITING:
        if (time >= TYPEWRITING_INTERVAL) {
            time = 0;
            while (i++ < STRING.length());

            if (i == STRING.length()) {
                state = WAITING;
                time = 0;
            }
        }
        break;

    case WAITING:
        if (time >= TIME_WAITING) {
            g.next_screen = new SplashScreen;
            g.next_screen->prev_screen = this;
            // app->stack.popThenPush(new SplashScreen, 2);
        }
        break;
    }
}

void OverScreen::process_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_RETURN) {
            time = TIME_WAITING;
            i = STRING.length();
        }
        break;

    case SDL_QUIT:
        g.next_screen = nullptr;
        break;
    }
}
