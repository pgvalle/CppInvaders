#pragma once

// common includes
#include <string>

#include <pico.h>
#include "Screens.h"

#define FRAMERATE 60

#define WHITE { 255, 255, 255, 255 }
#define GREEN { 32, 255, 32, 255 }
#define RED { 255, 32, 32, 255 }

#define IMG_CANNON "./res/img/cannon.png"
#define IMG_UFO "./res/img/ufo.png"
#define IMG_INV1 "./res/img/inv1.png"
#define IMG_INV2 "./res/img/inv2.png"
#define IMG_INV3 "./res/img/inv3.png"
#define IMG_EXP1 "./res/img/exp1.png"
#define IMG_EXP2 "./res/img/exp2.png"
#define IMG_EXP3 "./res/img/exp3.png"
#define FONT "./res/font.ttf"

class App {
public:
    int credits, score, hi_score;
    Screen *current_screen, *next_screen;

    App();
    ~App();

    void start();

    int load_hi_score();
    void save_hi_score();
    void draw_counters();

private:
    void update_and_draw(float delta);
    int process_frame_events(int timeout);
};

extern App *app;
