#include "CppInvaders.h"
#include "screens/Screens.h"

void CppInvaders::load_scoreboard() {
    score = 0;
    hi_score = 0;

    FILE *file = fopen(SCOREBOARD_FILE, "r");
    if (!file) {
        file = fopen(SCOREBOARD_FILE, "w");
        fclose(file);
        return;
    }

    if (!file) {
        printf("Couldn't open scoreboard file\n");
        return;
    }

    fscanf(file, "%6d", &hi_score);
    fclose(file);
}

void CppInvaders::save_scoreboard() {
    // open file to write (will exist bc load was called b4)
    // write hi_score to file
    // closefile
    FILE *file = fopen(SCOREBOARD_FILE, "w");
    if (!file) {
        printf("Couldn't open scoreboard file\n");
        return;
    }

    fprintf(file, "%6d", hi_score);
    fclose(file);
}

void CppInvaders::add_to_score(int value) {
    score += value;
    if (hi_score < score) {
        hi_score = score;
        save_scoreboard();
    }
}

void CppInvaders::draw_scoreboard() {
    static char text[32];
    sprintf(text, "%06d          %06d", score, hi_score);
    
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    fcd_draw_text(8, 8, "YOUR SCORE      HIGH-SCORE");
    fcd_draw_text(24, 24, text);
}

void CppInvaders::draw_credit_counter() {
    static char text[16];
    sprintf(text, "CREDIT %02d", credits);

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    fcd_draw_text(144, 240, text);
}

CppInvaders::CppInvaders() {
    splash = new Splash;
    game = nullptr;
    pause = nullptr;
    over = nullptr;

    screen = SPLASH;
    credits = 99;
    load_scoreboard();

    should_quit = false;
}

CppInvaders::~CppInvaders() {
    save_scoreboard();

    delete splash;
    delete game;
    delete pause;
    delete over;
}

void CppInvaders::update_and_draw(float delta) {
    switch (screen) {
    case SPLASH:
        SDL_assert(splash);
        splash->draw();
        splash->update(delta);
        break;
    case GAME:
        SDL_assert(game);
        game->draw();
        game->update(delta);
        break;
    case PAUSE:
        SDL_assert(pause);
        pause->draw();
        pause->update(delta);
        break;
    case OVER:
        SDL_assert(over);
        over->draw();
        over->update(delta);
        break;
    }

    SDL_RenderPresent(ren);
}

void CppInvaders::process_event(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        should_quit = true;
        return;
    }

    switch (screen) {
    case SPLASH:
        SDL_assert(splash);
        splash->process_event(event);
        break;
    case GAME:
        SDL_assert(game);
        game->process_event(event);
        break;
    case PAUSE:
        SDL_assert(pause);
        pause->process_event(event);
        break;
    case OVER:
        SDL_assert(over);
        over->process_event(event);
        break;
    }
}
