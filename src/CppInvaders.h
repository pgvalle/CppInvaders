#pragma once

#include <pico.h>

#define WHITE { 255, 255, 255, 255 }
#define GREEN { 32, 255, 32, 255 }
#define RED { 216, 32, 32, 255 }

#define SCOREBOARD_FILE "./scoreboard"

#define IMG_SPACESHIP "./res/img/cannon.png"
#define IMG_UFO "./res/img/ufo.png"
#define IMG_INV1 "./res/img/inv1.png"
#define IMG_INV2 "./res/img/inv2.png"
#define IMG_INV3 "./res/img/inv3.png"
#define IMG_EXP1 "./res/img/exp1.png"
#define IMG_EXP2 "./res/img/exp2.png"
#define IMG_EXP3 "./res/img/exp3.png"
#define FONT "./res/font.ttf"

struct CppInvaders {
private:
    class Splash;
    class Game;
    class Pause;
    class Over;

    Splash *splash;
    Game *game;
    Pause *pause;
    Over *over;

    enum Screen {
        SPLASH, GAME, PAUSE, OVER
    };
    
    Screen screen;
    int credits, score, hi_score;

    void load_scoreboard();
    void save_scoreboard();
    void add_to_score(int value);

    void draw_scoreboard();
    void draw_credit_counter();

public:
    bool should_quit;

    CppInvaders();
    ~CppInvaders();

    void update_and_draw(float delta);
    void process_event(const SDL_Event& event);
};

extern CppInvaders *cppinv;