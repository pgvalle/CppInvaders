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
    g.next_screen = g.screen;

    while (g.screen) {
        int timeout = 16, accum = 0;
        while (timeout > 0) {
            int before = pico_get_ticks();

            SDL_Event event;
            pico_input_event_timeout(&event, SDL_ANY, timeout);
            g.screen->process_event(event);

            int delta = pico_get_ticks() - before;
            timeout -= delta;
            accum += delta;
        }

        pico_assert(g.screen);
        g.screen->update(0.001f * accum); // to seconds
        pico_assert(g.screen);
        g.screen->draw();

        pico_output_draw_text({ 8, 8 }, "YOUR SCORE      HIGH-SCORE");
        char text[32];
        sprintf(text, "%06d          %06d", 123, 123);
        pico_output_draw_text({ 24, 24 }, text);
        pico_output_draw_text({ 144, 240 }, "CREDIT --");

        pico_output_present();
        
        if (g.screen != g.next_screen) {
            delete g.screen;
            g.screen = g.next_screen;
        }
    }

    pico_init(0);

    return 0;
}
