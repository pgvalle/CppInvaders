#pragma once

#include <pico.h>

#define ROUND(x) ((int)round(x))

#define WHITE { 255, 255, 255, 255 }
#define GREEN { 32, 255, 32, 255 }
#define RED { 216, 32, 32, 255 }

#define SCOREBOARD_FILE "./scoreboard"

#define IMGS "./res/img/"
#define SFXS "./res/sfx/"

#define IMG_SPACESHIP IMGS "spaceship.png"
#define IMG_UFO IMGS "ufo.png"
#define IMG_INV1 IMGS "inv1.png"
#define IMG_INV2 IMGS "inv2.png"
#define IMG_INV3 IMGS "inv3.png"
#define IMG_EXP1 IMGS "exp1.png"
#define IMG_EXP2 IMGS "exp2.png"
#define IMG_EXP3 IMGS "exp3.png"

#define SFX_SPACESHIP_SHOOT SFXS "spaceship-shoot.wav"
#define SFX_SPACESHIP_KILLED SFXS "spaceship-killed.wav"
#define SFX_INVADER_KILLED SFXS "invader-killed.wav"

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

    void draw_scoreboard();
    void draw_credit_counter();

public:
    bool should_quit;

    CppInvaders();
    ~CppInvaders();

    void add_to_score(int value);

    void update_and_draw(float delta);
    void process_event(const SDL_Event& event);
};

extern CppInvaders *cppinv;
