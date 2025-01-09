#include "SplashScreen.h"
#include "../global.h"
//#include "PlayScreen.h"
#include <string>

#define TYPEWRITING_INTERVAL 0.05f
#define TIME_WAITING 1.5f
#define TYPEWRITE1_INDEX_STOP SCREEN_STR.find('*')

// TODO: Find another way to create a new line because pico doesn't like \n.
static std::string SCREEN_STR =
    "         PLAY\n\n\n"
    "    SPACE INVADERS\n\n\n\n"
    "*SCORE ADVANCES TABLE*\n\n"
    "       =? MYSTERY\n\n"
    "       =30 POINTS\n\n"
    "       =20 POINTS\n\n"
    "       =10 POINTS";

SplashScreen::SplashScreen() {
    state = WAITING1;
    i = 1;
    time = 0;
}

SplashScreen::~SplashScreen() {}

void SplashScreen::draw() {
    pico_set_color_clear({ 0x0, 0x0, 0x0, 0x0 });
    pico_output_clear();

    //ui_draw();

    std::string screen_substr = SCREEN_STR.substr(0, i);
    pico_output_draw_text({ 24, 64 }, (char *)screen_substr.c_str());

    if (i > TYPEWRITE1_INDEX_STOP) {
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

void SplashScreen::update(float delta) {
    time += delta;

    switch (state) {
    case WAITING1:
        if (time >= TIME_WAITING) {
            state = TYPEWRITING1;
            time = 0;
        }
        break;

    case TYPEWRITING1:
        if (time >= TYPEWRITING_INTERVAL) {
            time = 0;
            // skip spaces and escape sequences
            while (++i < TYPEWRITE1_INDEX_STOP && SCREEN_STR[i] <= ' ');
        }

        if (i == TYPEWRITE1_INDEX_STOP) {
            state = WAITING2;
            time = 0;
            i--;
        }
        break;

    case WAITING2:
        if (time >= TIME_WAITING) {
            state = TYPEWRITING2;
            time = 0;
            i += 23;
        }
        break;

    case TYPEWRITING2:
        if (time >= TYPEWRITING_INTERVAL) {
            time = 0;
            // skip spaces and escape sequences
            while (++i < SCREEN_STR.length() && SCREEN_STR[i] <= ' ');
        }

        if (i == SCREEN_STR.length()) {
            state = WAITING_KEYPRESS;
        }
        break;

    case WAITING_KEYPRESS:
        break;
    }
}

void SplashScreen::process_event(const SDL_Event &event)
{
    switch (event.type)
    {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym != SDLK_RETURN)
            break;

        if (state == WAITING_KEYPRESS) {
            g.score = 0;
            g.hi_score = 0;
            //app->stack.popThenPush(new PlayScreen(3));
            break;
        }

        state = WAITING_KEYPRESS;
        i = SCREEN_STR.length();
        break;

    case SDL_QUIT:
        //app->should_quit = true;
        break;
    }
}
