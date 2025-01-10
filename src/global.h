#pragma once

#include "Screen.h"

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

struct Global {
    Screen *screen, *next_screen;
    int score, hi_score;
};

extern Global g;