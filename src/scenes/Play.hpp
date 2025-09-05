#pragma once

#include "Entity.hpp"
#include "scenes/play/UFO.hpp"
#include "scenes/play/Spaceship.hpp"

class PlayScene : public Entity {
private:
    Spaceship s;
    UFO ufo;

public:
    PlayScene();
    virtual ~PlayScene();

    void process_event(const Pico_Event &event) override;
    void update(float delta) override;
    void draw() const override;
};
