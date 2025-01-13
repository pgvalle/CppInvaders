#pragma once

//#include "entts/Cannon.h"
//#include "entts/Horde.h"
//#include "entts/UFO.h"
//#include "entts/Bunker.h"

#include "CppInvaders.h"

#define GAMESCOPE CppInvaders::Game

class CppInvaders::Game {
private:
    enum State {
        POPULATING_HORDE,
        WAITING_CANNON,
        PLAYING,
        CANNON_EXPLODING,
        CANNON_DEAD,
        RESTARTING
    };

    State state;

    struct UFO;
    //struct Horde;
    struct Bunker;
    struct Spaceship;
    struct Shot;

    //float time;
    UFO *ufo;
    //Horde *horde;
    //Bunker *bunkers[4];
    Spaceship *spaceship;
    Shot *horde_shot, *spaceship_shot;

    void process_collisions(float delta);

public:
    Game();
    ~Game();

    void draw();
    void update(float delta);
    void process_event(const SDL_Event &event);
};
