#include "Pause.hpp"
#include "../CppInvaders.hpp"

#define PAUSE_SYMBOL_BLINK 0.5
#define TIME_TO_RESUME 3

PauseScene::PauseScene(Entity *gameplay$) {
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
      resuming = (resuming ? !resuming : resuming);
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
    CppInvaders::get_ref().scene = gameplay;
    delete this;
    return;
  }

  if (!resuming && timer >= PAUSE_SYMBOL_BLINK) {
    timer = 0;
    pause_symbol = !pause_symbol;
    return;
  }
}

void PauseScene::draw() const {
  //gameplay->draw();
  // make the pause menu effect
  pico_set_color_draw({0, 0, 0, 204});
  pico_output_draw_rect({0, 0, 224, 256});

  pico_set_color_draw(WHITE);
  if (resuming) {
    static char text[16];
    sprintf(text, "%02d", 3 - (int)timer);
    pico_output_draw_text({104, 8}, text);
    return;
  }
  
  if (pause_symbol) {
    pico_output_draw_text({104, 8}, "||");
    return;
  }
}
