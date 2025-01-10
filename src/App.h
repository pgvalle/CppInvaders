#pragma once

#include "Screen.h"

class App {
public:
    int credits, score, hi_score;
    Screen *current_screen, *next_screen;

    App();
    ~App();

    void start();

    int process_frame_events(int timeout);
    void update_and_draw(float delta);
    void draw_counters();
    int load_hi_score();
    void save_hi_score();
};

