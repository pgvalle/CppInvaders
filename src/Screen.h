#pragma once

#include <pico.h>

class Screen
{
protected:
    Screen *prev_screen = nullptr;

public:
    virtual ~Screen() {};

    virtual void draw() = 0;
    virtual void update(float delta) = 0;
    virtual void process_event(const SDL_Event &event) = 0;
};