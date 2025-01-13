#pragma once

//#include "entts/Cannon.h"
//#include "entts/Horde.h"
#include "entts/Shot.h"
#include "entts/UFO.h"
//#include "entts/Bunker.h"

#include "CppInvaders.h"
#include <vector>

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
    //float time;
    //UFO ufo;
    //Horde horde;
    //Cannon cannon;
    //Bunker bunkers[4];
    std::vector<Shot *> spaceship_shots, horde_shots;

    void update_shots(float delta);
    void process_collisions(float delta);

public:
    Game();
    ~Game();

    void draw();
    void update(float delta);
    void process_event(const SDL_Event &event);
};
