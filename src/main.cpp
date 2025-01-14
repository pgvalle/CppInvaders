#include "CppInvaders.h"
#include <ctime>

CppInvaders *cppinv = nullptr;

void init();
void quit();
void loop();

int main(int argc, char **argv) {
    init();
    loop();
    quit();

    return 0;
}

void init() {
    srand(time(nullptr));

    pico_init(true);
    
    pico_set_title("CppInvaders");
    pico_set_size_internal({ 224, 256 });
    pico_set_size_external({ 448, 512 });

    pico_set_grid(false);
    pico_set_anchor(PICO_LEFT, PICO_TOP);
    
    pico_set_font(FONT, 8);

    cppinv = new CppInvaders;
}

void quit() {
    delete cppinv;
    pico_init(false);
}

void loop() {
    pico_assert(cppinv);

    while (!cppinv->should_quit) {
        int timeout = 16, accum = 0;
        while (timeout > 0) {
            int before = pico_get_ticks();

            SDL_Event event;
            pico_input_event_timeout(&event, SDL_ANY, timeout);
            cppinv->process_event(event);

            int delta = pico_get_ticks() - before;
            timeout -= delta;
            accum += delta;
        }

        float delta = 0.001f * accum;
        cppinv->update_and_draw(delta);
        pico_output_present();
    }
}
