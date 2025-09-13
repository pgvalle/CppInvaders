#pragma once

#include "Entity.hpp"
#include "assets.hpp"

#define WHITE {255, 255, 255, 255}
#define GREEN {32, 255, 32, 255}
#define RED {216, 32, 32, 255}

#define SCOREBOARD_FILE "./scoreboard"

class CppInvaders : private Entity {
private:

    CppInvaders();
    virtual ~CppInvaders();

    void loop();
    void process_event(const Pico_Event &event) override;
    void update(float delta) override;
    void draw() const override;

public:
    Entity *scene;
    bool should_quit;
    int score, hi_score, credits;
    int lives;

    void add_to_score(int value);
    void save_hi_score() const;
    void draw_indicators() const;
    static CppInvaders &get();
    static void main();
};
