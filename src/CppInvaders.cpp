#include "CppInvaders.h"
#include "screens/Splash.h"
#include "screens/Game.h"
#include "screens/Over.h"

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
    
    pico_set_color_draw(WHITE);
    
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

    should_quit = false;
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
        pico_assert(splash);
        splash->draw();
        splash->update(delta);
        break;
    case GAME:
        pico_assert(game);
        game->draw();
        game->update(delta);
        break;
    case PAUSE:
        // pico_assert(pause);
        // pause->draw();
        // pause->update(delta);
        break;
    case OVER:
        pico_assert(over);
        over->draw();
        over->update(delta);
        break;
    }
}

void CppInvaders::process_event(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        should_quit = true;
        return;
    }

    switch (screen) {
    case SPLASH:
        pico_assert(splash);
        splash->process_event(event);
        break;
    case GAME:
        pico_assert(game);
        game->process_event(event);
        break;
    case PAUSE:
        // pico_assert(pause);
        // pause->process_event(event);
        break;
    case OVER:
        pico_assert(over);
        over->process_event(event);
        break;
    }
}
