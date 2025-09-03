#pragma once

#include "../Entity.hpp"

class PlayScene : public Entity {
private:
public:
    PlayScene();
    virtual ~PlayScene();

    void process_event(const Pico_Event &event) override;
    void update(float delta) override;
    void draw() const override;
};
