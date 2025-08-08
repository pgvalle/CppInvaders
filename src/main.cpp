#include "CppInvaders.hpp"
#include "pico.h"

static CppInvaders *ref = nullptr;

CppInvaders &CppInvaders::get_ref() {
  return *ref;
}

void CppInvaders::main() {
  if (!ref) {
    ref = new CppInvaders;
    ref->run();
    delete ref;
    ref = nullptr;
  }
}

int main() {
  pico_init(1);
  pico_set_expert(1);
  pico_set_font("res/font.ttf", 8);
  pico_set_size({448,512}, {224,256});
  CppInvaders::main();
  pico_init(0);
  return 0;
}