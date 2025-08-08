#include "Over.hpp"
#include "../CppInvaders.hpp"
#include "Pause.hpp"
#include <string>

#define TYPEWRITING_INTERVAL 0.2f
#define TIME_WAITING 2.5f

static std::string STRING = " GAME OVER";

OverScene::OverScene(Scene *gameplay$) {
  gameplay = gameplay$;
  waiting = false;
  timer = 0;
  i = 0;
}

OverScene::~OverScene() {}

void OverScene::process_event(const SDL_Event &event) {
  switch (event.type) {
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_RETURN) {
      if (!waiting) {
        waiting = true;
        timer = 0;
        i = STRING.length();
      } else {
        timer = TIME_WAITING;
      }
    }
    break;
  }
}


void OverScene::update(float delta) {
  timer += delta;

  if (!waiting && timer >= TYPEWRITING_INTERVAL) {
    waiting = i++ == STRING.length();
    timer = 0;
    return;
  }

  if (waiting && timer >= TIME_WAITING) {
    CppInvaders::get_ref().scene = nullptr; // TODO: new SplashScene here
    delete gameplay;
    delete this;
    return;
  }
}

void OverScene::draw() const {
  if (gameplay) {
    gameplay->draw();
  }

  // pause menu dim effect
  pico_set_color_draw({0, 0, 0, 204});
  pico_set_anchor({PICO_LEFT, PICO_TOP});
  pico_output_draw_rect({0, 0, 224, 256});

  std::string str = (i == 0 ? " " : STRING.substr(0, i)); // len 0 str invalid
  Pico_Pos pos = pico_pos(50, 0);
  pos.y = 64;
  pico_set_color_draw(RED);
  pico_set_anchor({PICO_CENTER, PICO_TOP});
  pico_output_draw_text(pos, str.c_str());
}