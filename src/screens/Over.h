#pragma once

#include "CppInvaders.h"

class CppInvaders::Over {
private:
    enum State {
        TYPEWRITING, WAITING
    };

    State state;
    int i;
    float time;

public:
    Over();
    ~Over();

    void draw();
    void update(float delta);
    void process_event(const SDL_Event &event);
};
