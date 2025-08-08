#pragma once

#include "../Entity.hpp"

class OverScene : public Entity {
private:
  Scene *gameplay;
  bool waiting;
  float timer;
  int i;

public:
  OverScene(Scene *gameplay);
  virtual ~OverScene();

  void process_event(const Pico_Event &event) override;
  void update(float delta) override;
  void draw() const override;
};
