#pragma once

#include "Entity.hpp"

#define ROUND(x) ((int)round(x))
#define FRAMERATE 60

#define WHITE {255, 255, 255, 255}
#define GREEN {32, 255, 32, 255}
#define RED {216, 32, 32, 255}

#define SCOREBOARD_FILE "./scoreboard"

#define IMG "./res/img/"
#define SFX "./res/sfx/"
#define FONT "./res/font.ttf"

#define IMG_SPACESHIP IMG "spaceship.png"
#define IMG_UFO IMG "ufo.png"
#define IMG_INV1 IMG "inv1.png"
#define IMG_INV2 IMG "inv2.png"
#define IMG_INV3 IMG "inv3.png"
#define IMG_EXP1 IMG "exp1.png"
#define IMG_EXP2 IMG "exp2.png"
#define IMG_EXP3 IMG "exp3.png"

#define SFX_SPACESHIP_SHOOT SFX "spaceship-shoot.wav"
#define SFX_SPACESHIP_KILLED SFX "spaceship-killed.wav"
#define SFX_INVADER_KILLED SFX "invader-killed.wav"

class CppInvaders : private Entity {
private:
  CppInvaders();
  virtual ~CppInvaders();

  void run();
  void process_event(const Pico_Event &event) override;
  void update(float delta) override;
  void draw() const override;

public:
  bool should_quit;
  Entity *scene;
  int credits, score, hi_score;

  static CppInvaders &get_ref();
  static void main();
  void draw_indicators() const;
};
