#include "CppInvaders.hpp"
#include "scenes/Pause.hpp"

CppInvaders::CppInvaders() {
  should_quit = false;
  scene = new PauseScene(nullptr);
  credits = 0;
  score = 0;
  hi_score = 0; // load hi-score
}

CppInvaders::~CppInvaders() {
  // save hi-score
}

void CppInvaders::draw_indicators() const {
  static char fmt[16];
  Pico_Pos pos;
  
  pico_set_color_draw(WHITE);

  // credit counter
  pos = pico_pos(100, 100);
  sprintf(fmt, "CREDIT %02d", credits);
  pico_set_anchor({PICO_RIGHT, PICO_BOTTOM});
  pico_output_draw_text({pos.x - 8, pos.y - 8}, fmt);

  // score
  pico_set_anchor({PICO_LEFT, PICO_TOP});
  pico_output_draw_text({8, 8}, "YOUR SCORE");

  // hi-score
  pos = pico_pos(100, 0);
  pico_set_anchor({PICO_RIGHT, PICO_TOP});
  pico_output_draw_text({pos.x - 8, 8}, "HIGH-SCORE");
}

#define FRAMETIME (1000 / FRAMERATE)

void CppInvaders::run() {
  int delta = 0;
  while (!should_quit) {
    int start = pico_get_ticks();
    int passed = 0;
    while (passed < FRAMETIME) {
      Pico_Event event;
      int timeout = FRAMETIME - passed;
      if (pico_input_event_timeout(&event, PICO_ANY, timeout)) {
        process_event(event);
      }
      passed = pico_get_ticks() - start;
    }
    update(0.001f * delta);
    draw();
    delta = pico_get_ticks() - start;
  }
}

void CppInvaders::process_event(const Pico_Event &event) {
  if (event.type == PICO_QUIT) {
    should_quit = true;
  } else {
    scene->process_event(event);
  }
}

void CppInvaders::update(float delta) {
  scene->update(delta);
}

void CppInvaders::draw() const {
  pico_output_clear();
  scene->draw();
  draw_indicators();
  pico_output_present();
}