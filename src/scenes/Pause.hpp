#pragma once

#include "../Entity.hpp"

class PauseScene : public Entity {
private:
    Entity *gameplay;
    bool resuming;
    float timer;
    bool pause_symbol;

public:
    PauseScene(Entity *gameplay);
    virtual ~PauseScene();

    void process_event(const Pico_Event &event) override;
    void update(float delta) override;
    void draw() const override;
};
