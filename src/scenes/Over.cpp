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

void OverScene::process_event(const Pico_Event &event) {
    switch (event.type) {
    case PICO_EVENT_KEY_DOWN:
        if (event.key.keysym.sym == PICOK_RETURN) {
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

    pico_push();
    // pause menu dim effect
    pico_set_color_draw((Pico_Color){0, 0, 0});
    pico_set_alpha(204);
    Pico_Rel_Rect dim_r = { '%', {0.0f, 0.0f, 1.0f, 1.0f}, PICO_ANCHOR_NW, NULL };
    pico_output_draw_rect(&dim_r);
    pico_pop();

    std::string str = STRING.substr(0, ci);
    Pico_Rel_Rect cnt_r = { '%', {0.5f, 0.5f, 0, 0}, PICO_ANCHOR_C, NULL }; 
    if (!str.empty()) {
        pico_set_color_draw(PICO_COLOR_RED);
        Pico_Rel_Rect txt_r = { '#', {0.0f, 0.0f, 0.0f, 1.0f}, PICO_ANCHOR_C, &cnt_r };
        pico_output_draw_text(str.c_str(), &txt_r);
    }
}
