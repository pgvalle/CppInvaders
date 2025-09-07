#include "Over.hpp"
#include "../CppInvaders.hpp"
#include "Splash.hpp"
#include "pico.h"
#include <string>

#define TIME_TYPEWRITE 0.2f
#define TIME_WAITING 2.5f

static std::string STRING = "GAME OVER";

OverScene::OverScene(Scene *gameplay$) {
    gameplay = gameplay$;
    waiting = false;
    timer = 0;
    ci = 0;
}

OverScene::~OverScene() {
    delete gameplay;
}

void OverScene::process_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_RETURN) {
            if (!waiting) {
                waiting = true;
                timer = 0;
                ci = STRING.length();
            } else {
                timer = TIME_WAITING;
            }
        }
        break;
    }
}


void OverScene::update(float delta) {
    timer += delta;

    if (!waiting && timer >= TIME_TYPEWRITE) {
        waiting = ci++ == STRING.length();
        timer -= TIME_TYPEWRITE;
        return;
    }

    if (waiting && timer >= TIME_WAITING) {
        CppInvaders::get().scene = new SplashScene;
        delete this;
        return;
    }
}

void OverScene::draw() const {
    if (gameplay) {
        gameplay->draw();
    }

    // pause menu dim effect
    Pico_Dim dim = pico_dim({100, 100});
    pico_set_color_draw({0, 0, 0, 204});
    pico_set_anchor_draw({PICO_LEFT, PICO_TOP});
    pico_output_draw_rect({0, 0, dim.x, dim.y});

    std::string str = STRING.substr(0, ci);
    pico_set_color_draw(RED);
    pico_set_anchor_draw({PICO_LEFT, PICO_TOP});
    pico_output_draw_text({80, 64}, str.c_str());
}
