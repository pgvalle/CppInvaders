#include "SplashScreen.h"
#include "../global.h"
//#include "PlayScreen.h"
#include <string>

#define TYPEWRITING_INTERVAL 0.05f
#define TIME_WAITING 1.5f

static std::string STRINGS[7] = {
    "PLAY",
    "SPACE INVADERS",
    "*SCORE ADVANCES TABLE*",
    "=? MYSTERY",
    "=30 POINTS",
    "=20 POINTS",
    "=10 POINTS",
};

static int STRINGS_XOFF[7] = { 9, 4, 0, 7, 7, 7, 7 };
static int STRINGS_YOFF[7] = { 0, 3, 7, 9, 11, 13, 15 };

SplashScreen::SplashScreen() {
    state = WAITING1;
    i = 0;
    j = 0;
    time = 0;
}

SplashScreen::~SplashScreen() {}

void SplashScreen::draw() {
    pico_set_color_clear({ 0x0, 0x0, 0x0, 0x0 });
    pico_output_clear();

    //ui_draw();

    for (int k = 0; k < i; k++) {
        std::string &str = STRINGS[k];
        int xoff = 24 + 8 * STRINGS_XOFF[k];
        int yoff = 64 + 8 * STRINGS_YOFF[k];
        pico_output_draw_text({ xoff, yoff }, (char *)str.c_str());
    }

    if (j > 0) {
        std::string str = STRINGS[i].substr(0, j);
        int xoff = 24 + 8 * STRINGS_XOFF[i];
        int yoff = 64 + 8 * STRINGS_YOFF[i];
        pico_output_draw_text({ xoff, yoff }, (char *)str.c_str());
    }
    

    if (i > 2 && j > 0) {
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
            // increment word count
            if (j++ == STRINGS[i].length()) {
                i++;
                j = 0;
            }
        }

        if (i == 3) { // i = 2 and j == length str[i]
            state = WAITING2;
            time = 0;
        }
        break;

    case WAITING2:
        if (time >= TIME_WAITING) {
            state = TYPEWRITING2;
            time = 0;
        }
        break;

    case TYPEWRITING2:
        if (time >= TYPEWRITING_INTERVAL) {
            time = 0;
            // increment word count
            if (j++ == STRINGS[i].length()) {
                i++;
                j = 0;
            }
        }

        if (i == 7) { // i = 6 and j == length str[i]
            i = 6;
            j = STRINGS[i].length();
            state = WAITING_KEYPRESS;
        }
        break;

    case WAITING_KEYPRESS:
        break;
    }
}

void SplashScreen::process_event(const SDL_Event &event)
{
    switch (event.type) {
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
        i = 6;
        j = STRINGS[i].length();
        break;

    case SDL_QUIT:
        //app->should_quit = true;
        break;
    }
}
