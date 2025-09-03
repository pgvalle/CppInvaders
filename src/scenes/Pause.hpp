#pragma once

#include "../Entity.hpp"

class PauseScene : public Entity {
private:
    Scene *gameplay;
    bool resuming;
    float timer;
    bool pause_symbol;

public:
    PauseScene(Scene *gameplay);
    virtual ~PauseScene();

    void process_event(const Pico_Event &event) override;
    void update(float delta) override;
    void draw() const override;
};
