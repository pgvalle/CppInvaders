#pragma once

#include "CppInvaders.h"

class CppInvaders::Pause {
private:
    enum State {
        PAUSED,
        RESUMING
    };

    State state;
    float time;
    bool pause_symbol;

public:
    Pause();
    ~Pause();

    void draw();
    void update(float delta);
    void process_event(const SDL_Event &event);
};
