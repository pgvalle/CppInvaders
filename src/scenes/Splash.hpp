#pragma once

#include "../Entity.hpp"

class SplashScene : public Scene {
private:
  enum State {
    WAITING1,
    TYPEWRITING1,
    WAITING2,
    TYPEWRITING2,
    WAITING_KEYPRESS
  } state;
  float timer;
  int li, ci;

  bool typewrite_next_char();

public:
  SplashScene();
  virtual ~SplashScene();

  void process_event(const Pico_Event &event) override;
  void update(float delta) override;
  void draw() const override;
};
