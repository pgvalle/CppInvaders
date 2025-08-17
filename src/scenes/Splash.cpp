#include "Splash.hpp"
#include "../CppInvaders.hpp"
#include "Pause.hpp"
#include "pico.h"
#include <string>

#define TYPEWRITE_STEP_DELTA 0.05f
#define TIME_WAITING 1.5f

static std::string LINES[] = {"PLAY",
                              "SPACE INVADERS",
                              "*SCORE ADVANCES TABLE*",
                              "=? MYSTERY",
                              "=30 POINTS",
                              "=20 POINTS",
                              "=10 POINTS",
                              ""};

static Pico_Pos OFFS[] = {{72, 0},  {32, 24},  {0, 56},   {56, 72},
                          {56, 88}, {56, 104}, {56, 120}, {0, 0}};

SplashScene::SplashScene() {
  state = WAITING1;
  timer = 0;
  li = 0;
  ci = 0;
}

SplashScene::~SplashScene() {}

bool SplashScene::typewrite_next_char() {
  if (timer < TYPEWRITE_STEP_DELTA) {
    return false;
  }

  if (ci++ == (int)LINES[li].length()) {
    li++;
    ci = 0;
  }
  timer = 0;
  return true;
}

void SplashScene::process_event(const Pico_Event &event) {
  if (event.type != SDL_KEYDOWN || event.key.keysym.sym != SDLK_RETURN) {
    return;
  }

  if (state == WAITING_KEYPRESS) {
    CppInvaders &game = CppInvaders::get_ref();
    game.scene = new PauseScene(nullptr); // TODO: GameScene here
    game.score = 0;
    delete this;
  } else {
    state = WAITING_KEYPRESS;
    li = 7;
    ci = 0;
  }
}

void SplashScene::update(float delta) {
  timer += delta;

  if ((state == WAITING1 || state == WAITING2) && timer >= TIME_WAITING) {
    state = (state == WAITING1 ? TYPEWRITING1 : TYPEWRITING2);
    timer = 0;
    return;
  }

  if (state == TYPEWRITING1 && typewrite_next_char() && li == 2) {
    state = WAITING2;
    return;
  }

  if (state == TYPEWRITING2 && typewrite_next_char() && li == 7) {
    state = WAITING_KEYPRESS;
    return;
  }
}

void SplashScene::draw() const {
  pico_set_color_draw(WHITE);
  pico_set_anchor({PICO_LEFT, PICO_TOP});
  for (int lj = 0; lj <= li; lj++) {
    Pico_Pos pos = {24 + OFFS[lj].x, 64 + OFFS[lj].y};
    std::string str = LINES[lj];
    if (lj == li) {
      str = str.substr(0, ci);
    }
    pico_output_draw_text(pos, str.c_str());
  }

  if (state == TYPEWRITING2 || state == WAITING_KEYPRESS) {
    pico_set_anchor({PICO_CENTER, PICO_MIDDLE});
    pico_set_image_crop({0, 0, 24, 8});
    pico_output_draw_image({71, 140}, IMG_UFO);
    pico_set_image_crop({0, 0, 8, 8});
    pico_output_draw_image({71, 156}, IMG_INV1);
    pico_set_image_crop({0, 0, 11, 8});
    pico_output_draw_image({71, 172}, IMG_INV2);
    pico_set_image_crop({0, 0, 12, 8});
    pico_output_draw_image({71, 188}, IMG_INV3);
  }
}