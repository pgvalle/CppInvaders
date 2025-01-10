#pragma once

#include <pico.h>

class Screen
{
public:
    Screen *prev_screen = nullptr;

    virtual ~Screen() {};

    virtual void draw() = 0;
    virtual void update(float delta) = 0;
    virtual void process_event(const SDL_Event &event) = 0;
};