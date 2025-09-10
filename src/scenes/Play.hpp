#pragma once

#include <vector>
#include "Entity.hpp"
#include "scenes/play/UFO.hpp"
#include "scenes/play/Horde.hpp"
#include "scenes/play/Bullet.hpp"
#include "scenes/play/Spaceship.hpp"

class PlayScene : public Entity {
private:
    UFO ufo;
    Horde horde;
    std::vector<Bullet*> bullets;
    Spaceship ship;

    void process_collisions();

public:
    PlayScene();
    virtual ~PlayScene();
    void process_event(const Pico_Event &event) override;
    void update(float delta) override;
    void draw() const override;
};
