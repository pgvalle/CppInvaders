#pragma once

union SDL_Event;

class Screen {
public:
    virtual ~Screen() {};

    virtual void draw() = 0;
    virtual void update(float delta) = 0;
    virtual void process_event(const SDL_Event &event) = 0;
};