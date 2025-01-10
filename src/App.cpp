#include "App.h"
#include "global.h"

App *app = nullptr;

App::App() {
    credits = 1;
    score = 0;
    hi_score = load_hi_score();

    current_screen = nullptr;
    next_screen = nullptr;
}

App::~App() {
    save_hi_score();
}

void App::start() {
    next_screen = current_screen;

    while (current_screen) {
        int msdelta = process_frame_events(1000 / FRAMERATE);
        update_and_draw(0.001f * msdelta);
        
        if (current_screen != next_screen) {
            delete current_screen;
            current_screen = next_screen;
        }
    }
}

int App::process_frame_events(int timeout) {
    int accum = 0;

    while (timeout > 0) {
        int before = pico_get_ticks();

        SDL_Event event;
        pico_input_event_timeout(&event, SDL_ANY, timeout);
        assert(current_screen);
        current_screen->process_event(event);

        int delta = pico_get_ticks() - before;
        timeout -= delta;
        accum += delta;
    }

    return accum;
}

void App::update_and_draw(float delta) {
    assert(current_screen);
    current_screen->update(delta);

    assert(current_screen);
    current_screen->draw();

    pico_output_present();
}

void App::draw_counters() {
    static char text[32];
    
    pico_output_draw_text({ 8, 8 }, "YOUR SCORE      HIGH-SCORE");
    sprintf(text, "%06d          %06d", score, hi_score);
    pico_output_draw_text({ 24, 24 }, text);

    sprintf(text, "CREDIT %02d", credits);
    pico_output_draw_text({ 144, 240 }, text);
}


int App::load_hi_score() {
    // create file if it doesn't exist

    // read from it
    // try converting to an int
    return 0;
}

void App::save_hi_score() {

}