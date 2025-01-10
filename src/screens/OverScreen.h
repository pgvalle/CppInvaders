#pragma once

#include "../Screen.h"

class OverScreen : public Screen {
private:
    enum State {
        TYPEWRITING,
        WAITING
    } state;
    int i;
    float time;

public:
    OverScreen();
    ~OverScreen();

    void draw() override;
    void update(float delta) override;
    void process_event(const SDL_Event &event) override;
};
