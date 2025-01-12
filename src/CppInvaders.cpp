#include "CppInvaders.h"
#include "screens/Splash.h"

int CppInvaders::load_hi_score() {
    // create file if it doesn't exist

    // read from it
    // try converting to an int
    // close file
    return 0;
}

void CppInvaders::save_hi_score() {
    // open file to write (will exist bc load was called b4)
    // write hi_score to file
    // closefile
    printf("Score saved sucessfully!\n");
}

void CppInvaders::draw_counters() {
    static char text[32];
    
    pico_output_draw_text({ 8, 8 }, "YOUR SCORE      HIGH-SCORE");

    sprintf(text, "%06d          %06d", score, hi_score);
    pico_output_draw_text({ 24, 24 }, text);

    sprintf(text, "CREDIT %02d", credits);
    pico_output_draw_text({ 144, 240 }, text);
}

CppInvaders::CppInvaders() {
    splash = new Splash;
    game = nullptr;
    pause = nullptr;
    over = nullptr;

    screen = SPLASH;
    credits = 99;
    score = 0;
    hi_score = load_hi_score();

    should_close = false;
}

CppInvaders::~CppInvaders() {
    save_hi_score();

    delete splash;
    delete game;
    delete pause;
    delete over;
}

void CppInvaders::update_and_draw(float delta) {
    switch (screen) {
    case SPLASH:
        splash->update(delta);
        splash->draw();
        break;
    case GAME:
        break;
    case PAUSE:
        break;
    case OVER:
        break;
    }
}

void CppInvaders::process_event(const SDL_Event& event) {
    switch (screen) {
    case SPLASH:
        splash->process_event(event);
        break;
    case GAME:
        break;
    case PAUSE:
        break;
    case OVER:
        break;
    }
}
