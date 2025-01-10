#pragma once

#include "../global.h"

class SplashScreen : public Screen {
private:
    enum {
        WAITING1,
        TYPEWRITING1,
        WAITING2,
        TYPEWRITING2,
        WAITING_KEYPRESS
    } state;
    int l, c; // line iterator and character iterator for typewritting lines of text
    float time;

    bool typewrite_next_character();

public:
    SplashScreen();
    ~SplashScreen();

    void draw() override;
    void update(float delta) override;
    void process_event(const SDL_Event &event) override;
};
