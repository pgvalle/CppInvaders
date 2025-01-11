#include "App.h"

int main() {
    pico_init(true);
    
    pico_set_title("CppInvaders");
    pico_set_size_internal({ 224, 256 });
    pico_set_size_external({ 448, 512 });

    pico_set_grid(false);
    pico_set_anchor(PICO_LEFT, PICO_TOP);
    
    pico_set_font(FONT, 8);

    app = new App;
    app->current_screen = new SplashScreen;
    app->start();
    delete app;
    app = nullptr;

    pico_init(false);

    return 0;
}
