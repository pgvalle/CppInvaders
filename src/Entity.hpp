#pragma once

#include <pico.h>

class Entity {
public:
  virtual ~Entity() = default;

  virtual void process_event(const Pico_Event &event) = 0;
  virtual void update(float delta) = 0;
  virtual void draw() const = 0;
};

typedef Entity Scene;