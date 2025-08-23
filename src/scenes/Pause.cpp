#include "Pause.hpp"
#include "../CppInvaders.hpp"
#include "Over.hpp"

#define PAUSE_SYMBOL_BLINK 0.5
#define TIME_TO_RESUME 3

PauseScene::PauseScene(Scene *gameplay$) {
  gameplay = gameplay$;
  resuming = false;
  timer = 0;
  pause_symbol = true;
}

PauseScene::~PauseScene() {}

void PauseScene::process_event(const SDL_Event &event) {
  switch (event.type) {
  case SDL_KEYDOWN:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE: // start resume countdown or cancel it
      resuming = !resuming;
      timer = 0;
      pause_symbol = true;
      break;
    }
    break;
  }
}

void PauseScene::update(float delta) {
  timer += delta;

  if (resuming && timer >= TIME_TO_RESUME) {
    CppInvaders::get_ref().scene = new OverScene(this);
    //delete this;
    return;
  }

  if (!resuming && timer >= PAUSE_SYMBOL_BLINK) {
    timer = 0;
    pause_symbol = !pause_symbol;
    return;
  }
}

void PauseScene::draw() const {
  if (gameplay) {
    gameplay->draw();
  }

  // pause menu dim effect
  Pico_Dim dim = pico_dim(100, 100);
  pico_set_color_draw({0, 0, 0, 204});
  pico_set_anchor_draw({PICO_LEFT, PICO_TOP});
  pico_output_draw_rect({0, 0, dim.x, dim.y});

  Pico_Pos pos = {pico_pos(50, 0).x, 8};
  pico_set_color_draw(WHITE);
  pico_set_anchor_draw({PICO_CENTER, PICO_TOP});

  if (resuming) {
    static char fmt[16];
    sprintf(fmt, "%1d", 3 - (int)timer);
    pico_output_draw_text(pos, fmt);
    return;
  }

  if (pause_symbol) {
    pico_output_draw_text(pos, "||");
    return;
  }
}
