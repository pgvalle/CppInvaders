#pragma once

#include "../Screen.h"

class SplashScreen : public Screen {
private:
    enum State {
        WAITING1,
        TYPEWRITING1,
        WAITING2,
        TYPEWRITING2,
        WAITING_KEYPRESS
    } state;
    int i;
    float time;

public:
    SplashScreen();
    ~SplashScreen();

    void draw() override;
    void update(float delta) override;
    void process_event(const SDL_Event &event) override;
};
