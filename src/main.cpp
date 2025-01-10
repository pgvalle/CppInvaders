#include "global.h"
#include "screens/SplashScreen.h"

int main() {
    pico_init(true);

    pico_set_title("CppInvaders");
    pico_set_size_internal({ 224, 256 });
    pico_set_size_external({ 448, 512 });
    pico_set_grid(false);
    pico_set_anchor(PICO_LEFT, PICO_TOP);
    pico_set_font(FONT, 8);

    credits = 1;
    score = 0;
    hi_score = 0;

    current_screen = new SplashScreen;
    next_screen = current_screen;

    while (current_screen) {
        int timeout = 16, accum = 0;

        while (timeout > 0) {
            int before = pico_get_ticks();

            SDL_Event event;
            pico_input_event_timeout(&event, SDL_ANY, timeout);
            current_screen->process_event(event);

            int delta = pico_get_ticks() - before;
            timeout -= delta;
            accum += delta;
        }

        pico_assert(current_screen);
        current_screen->update(0.001f * accum);

        pico_assert(current_screen);
        current_screen->draw();
        pico_output_present();
        
        current_screen = next_screen;
    }

    pico_init(false);

    return 0;
}
