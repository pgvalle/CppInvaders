#include "CppInvaders.hpp"
#include "scenes/Pause.hpp"

CppInvaders::CppInvaders() {
  should_quit = false;
  scene = new PauseScene(nullptr);
}

CppInvaders::~CppInvaders() {
  // save score
}

void CppInvaders::run() {
  int delta = 0;
  while (!should_quit) {
    int before = pico_get_ticks(), passed = 0;
    while (passed < 16) {
      Pico_Event event;
      pico_input_event_timeout(&event, PICO_ANY, 16 - passed);
      process_event(event);
      passed = pico_get_ticks() - before;
    }
    update(0.001f * delta);
    printf("%d %f\n", delta, 0.001f*delta);
    draw();
    pico_output_present();
    delta = pico_get_ticks() - before;
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
  pico_output_present();
}