#pragma once

#include "Entity.hpp"
#include "assets.hpp"

#define ROUND(x) ((int)round(x))
#define FRAMERATE 60

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
    bool should_quit;
    Entity *scene;
    int credits, score, hi_score;

    void save_hi_score() const;
    void draw_indicators() const;
    static CppInvaders &get();
    static void main();
};
