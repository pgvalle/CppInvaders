#include "CppInvaders.hpp"
#include "scenes/Splash.hpp"

#define FRAMETIME (1000 / FPS)
static const int FPS = 60;

CppInvaders::CppInvaders() {
    scene = new SplashScene;
    should_quit = false;
    score = 0;
    hi_score = 0;
    credits = 0;
    lives = 3;

    FILE *file = fopen(SCOREBOARD_FILE, "a+");
    assert(file && "could not read scoreboard");
    fscanf(file, "%d", &hi_score);
    fclose(file);
}

CppInvaders::~CppInvaders() {
    save_hi_score();
}

void CppInvaders::loop() {
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
        draw();
        update(0.001f * delta);
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
    pico_set_crop({0, 0, 0, 0});
    pico_output_clear();
    draw_indicators();
    scene->draw();
    pico_output_present();
}

void CppInvaders::add_to_score(int value) {
    score += value;
    hi_score = SDL_max(score, hi_score);
}

void CppInvaders::save_hi_score() const {
    FILE *file = fopen(SCOREBOARD_FILE, "w+");
    assert(file && "could not save scoreboard");
    fprintf(file, "%d", hi_score);
    fclose(file);
}

void CppInvaders::draw_indicators() const {
    Pico_Pos pos;
    Pico_Dim dim = pico_get_size_text("##########");

    pico_set_color_draw(WHITE);

    // score
    pos = {8, 8};
    pico_set_anchor_draw({PICO_LEFT, PICO_TOP});
    pico_output_draw_text(pos, "YOUR SCORE");
    pos = pico_pos_ext({pos.x, pos.y, dim.x, dim.y}, {50, 200});
    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    pico_output_draw_fmt(pos, "%06d", score);

    // hi-score
    pos = {pico_pos({100, 0}).x - 8, 8};
    pico_set_anchor_draw({PICO_RIGHT, PICO_TOP});
    pico_output_draw_text(pos, "HIGH-SCORE");
    pos = pico_pos_ext({pos.x, pos.y, dim.x, dim.y}, {50, 200});
    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    pico_output_draw_fmt(pos, "%06d", hi_score);

    // credit counter
    pos = pico_pos({100, 100});
    pos = {pos.x - 8, pos.y - 8};
    pico_set_anchor_draw({PICO_RIGHT, PICO_BOTTOM});
    pico_output_draw_fmt(pos, "CREDIT %02d", credits);
}

// RUN HOOK

static CppInvaders *ref = nullptr;

CppInvaders &CppInvaders::get() {
    pico_assert(ref);
    return *ref;
}

void CppInvaders::main() {
    if (ref) {
        return;
    }

    pico_init(1);
    pico_set_title("CppInvaders");
    pico_set_grid(0);
    pico_set_expert(1);
    pico_set_font(FONT, 8);
    pico_set_size({2*224, 2*256}, {224, 256});
    ref = new CppInvaders;
    ref->loop();
    delete ref;
    ref = nullptr;
    pico_init(0);
}
