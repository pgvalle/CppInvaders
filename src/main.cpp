#include "global.h"
#include "screens/SplashScreen.h"

Global g;

int main() {
    pico_init(1);

    pico_set_title("CppInvaders");
    pico_set_size_internal({ 224, 256 });
    pico_set_size_external({ 448, 512 });
    pico_set_grid(0);
    pico_set_anchor(PICO_LEFT, PICO_TOP);
    pico_set_font(FONT, 8);

    g.score = 0;
    g.hi_score = 0;
    g.screen = new SplashScreen;

    while (1) {
        int delta = 16, time = 0;
        bool should_quit = false;
        while (delta > 0) {
            int old = SDL_GetTicks();
            SDL_Event e;
            pico_input_event_timeout(&e, SDL_ANY, delta);
            if (e.type == SDL_QUIT) {
                should_quit = true;
                break;
            }

            g.screen->process_event(e);

            int dt = SDL_GetTicks() - old;
            delta -= dt;
            time += dt;
        }

        if (should_quit) break;

        g.screen->update(0.001f * time);
        g.screen->draw();

        pico_output_draw_text({ 8, 8 }, "YOUR SCORE      HIGH-SCORE");
        char text[32];
        sprintf(text, "%06d          %06d", 123, 123);
        pico_output_draw_text({ 24, 24 }, text);
        pico_output_draw_text({ 144, 240 }, "CREDIT --");

        pico_output_present();
    }

    pico_init(0);

    return 0;
}
