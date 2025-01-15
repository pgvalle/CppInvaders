#pragma once

//#include "entts/Cannon.h"
//#include "entts/Horde.h"
//#include "entts/UFO.h"
//#include "entts/Bunker.h"

#include "CppInvaders.h"
#include "entts/Entities.h"

#define GAMESCOPE CppInvaders::Game
#define GAMEVAR cppinv->game

class CppInvaders::Game {
private:
    enum State {
        STARTING,
        PLAYING,
        RESTARTING,
        RESTARTING2  // TODO: Use better names for these states
    };

    State state;
    float time;
    bool horde_reached_spaceship;

    UFO *ufo;
    Horde *horde;
    Shield shields[4];
    Spaceship *spaceship;
    Shot *horde_shot, *spaceship_shot;

    void process_collisions();

public:
    Game();
    ~Game();

    void draw();
    void update(float delta);
    void process_event(const SDL_Event &event);
};
