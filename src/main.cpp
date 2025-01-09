#include <pico.h>
#include "global.h"

int main() {
    pico_init(1);

    pico_set_title("CppInvaders");
    pico_set_size_internal({ 224, 256 });
    pico_set_size_external({ 448, 512 });
    pico_set_grid(0);
    pico_set_anchor(PICO_LEFT, PICO_TOP);
    pico_set_font(FONT, 8);
    pico_set_color_clear({ 0x0, 0x0, 0x0, 0x0 });

    while (1) {
        int delta = 16;
        bool should_quit = false;
        while (delta > 0) {
            int old = SDL_GetTicks();
            SDL_Event e;
            pico_input_event_timeout(&e, SDL_ANY, delta);
            if (e.type == SDL_QUIT) {
                should_quit = true;
                break;
            }

            // processamento de input

            int dt = SDL_GetTicks() - old;
            delta -= dt;
        }

        if (should_quit) break;

        // processamento de eventos que dependem do tempo
        
        pico_output_clear();

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
