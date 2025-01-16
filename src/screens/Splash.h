#pragma once

#include "CppInvaders.h"

class CppInvaders::Splash {
private:
    enum State {
        WAITING1,
        TYPEWRITING1,
        WAITING2,
        TYPEWRITING2,
        WAITING_KEYPRESS
    };

    State state;
    int l, c; // line iterator and character iterator for typewritting lines of text
    float timer;

    bool typewrite_next_char();

public:
    Splash();
    ~Splash();

    void draw();
    void update(float delta);
    void process_event(const SDL_Event &event);
};
