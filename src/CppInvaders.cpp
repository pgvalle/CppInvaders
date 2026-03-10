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
            if (pico_input_event_timeout(&event, PICO_EVENT_ANY, timeout)) {
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
    if (event.type == PICO_EVENT_QUIT) {
        should_quit = true;
    } else {
        scene->process_event(event);
    }
}

void CppInvaders::update(float delta) {
    if (delta > 0.1f) delta = 0.1f;
    scene->update(delta);
}

void CppInvaders::draw() const {
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
    pico_set_color_draw(PICO_COLOR_WHITE);

    // score
    Pico_Rel_Rect tl_corner = { '%', {0.0f, 0.0f, 0.0f, 0.0f}, PICO_ANCHOR_NW, NULL };
    Pico_Rel_Rect score_lbl_r = { '#', {2.0f, 2.0f, 0.0f, 1.0f}, PICO_ANCHOR_NW, &tl_corner };
    pico_output_draw_text("YOUR SCORE", &score_lbl_r);

    char score_buf[16];
    snprintf(score_buf, sizeof(score_buf), "%06d", score);
    Pico_Rel_Rect score_val_r = { '#', {4.0f, 4.0f, 0.0f, 1.0f}, PICO_ANCHOR_NW, &score_lbl_r };
    pico_output_draw_text(score_buf, &score_val_r);

    // hi-score
    Pico_Rel_Rect tr_corner = { '%', {1.0f, 0.0f, 0.0f, 0.0f}, PICO_ANCHOR_NE, NULL };
    Pico_Rel_Rect hi_lbl_r = { '#', {-1.0f, 2.0f, 0.0f, 1.0f}, PICO_ANCHOR_NE, &tr_corner };
    pico_output_draw_text("HIGH-SCORE", &hi_lbl_r);

    char hi_buf[16];
    snprintf(hi_buf, sizeof(hi_buf), "%06d", hi_score);
    Pico_Rel_Rect hi_val_r = { '#', {-3.0f, 4.0f, 0.0f, 1.0f}, PICO_ANCHOR_NE, &hi_lbl_r };
    pico_output_draw_text(hi_buf, &hi_val_r);

    // credit counter
    Pico_Rel_Rect br_corner = { '%', {1.0f, 1.0f, 0.0f, 0.0f}, PICO_ANCHOR_SE, NULL };
    char credit_buf[16];
    snprintf(credit_buf, sizeof(credit_buf), "CREDIT %02d", credits);
    Pico_Rel_Rect credit_r = { '#', {-1.0f, -1.0f, 0.0f, 1.0f}, PICO_ANCHOR_SE, &br_corner };
    pico_output_draw_text(credit_buf, &credit_r);
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
    
    // Window: 448x512 physical
    pico_set_window("CppInvaders", -1, NULL);

    Pico_Rel_Dim win_dim = { '!', {448, 512}, NULL };
    pico_set_window(NULL, -1, &win_dim);

    // View: 224x256 logical
    Pico_Rel_Dim log_dim = { '!', {224, 256}, NULL };
    Pico_Abs_Dim tile_dim = { 8, 8 };
    pico_set_view(0, &log_dim, &tile_dim, NULL, NULL, NULL, NULL, NULL);

    pico_set_expert(1);
    pico_set_font(FONT);

    ref = new CppInvaders;
    ref->loop();
    delete ref;
    ref = nullptr;
    pico_init(0);
}
