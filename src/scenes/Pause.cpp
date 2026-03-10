#include "Pause.hpp"
#include "CppInvaders.hpp"

#define PAUSE_SYMBOL_BLINK 0.5
#define TIME_TO_RESUME 3

PauseScene::PauseScene(Scene *gameplay$) {
    gameplay = gameplay$;
    resuming = false;
    timer = 0;
    pause_symbol = true;
}

PauseScene::~PauseScene() {}

void PauseScene::process_event(const Pico_Event &event) {
    switch (event.type) {
    case PICO_EVENT_KEY_DOWN:
        switch (event.key.keysym.sym) {
            case PICOK_ESCAPE: // start resume countdown or cancel it
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
        CppInvaders::get().scene = gameplay;
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

    pico_set_color_draw(PICO_COLOR_WHITE);
    Pico_Rel_Rect txt_r = { '#', {14.0f, 4.0f, 0.0f, 1.0f}, PICO_ANCHOR_N, NULL };

    if (resuming) {
        char fmt[16];
        snprintf(fmt, sizeof(fmt), "%1d", 3 - (int)timer);
        pico_output_draw_text(fmt, &txt_r);
    } else if (pause_symbol) {
        pico_output_draw_text("||", &txt_r);
    }
}
