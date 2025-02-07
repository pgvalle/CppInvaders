#include "CppInvaders.h"
#include <ctime>

CppInvaders *cppinv = nullptr;

void init();
void quit();
void loop();

int main() {
    init();
    loop();
    quit();
    return 0;
}

void init() {
    srand(time(nullptr));

    SDL_assert(SDL_Init(SDL_INIT_VIDEO));
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);
    Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 4096);

    cppinv = new CppInvaders;
}

void quit() {
    delete cppinv;

    Mix_CloseAudio();
    TTF_Quit();
    SDL_Quit();
}

void loop() {
    while (!cppinv->should_quit) {
        int timeout = 16, accum = 0;
        while (timeout > 0) {
            int before = SDL_GetTicks();

            SDL_Event event;
            SDL_WaitEventTimeout(&event, timeout);
            cppinv->process_event(event);

            int delta = SDL_GetTicks() - before;
            timeout -= delta;
            accum += delta;
        }

        float delta = 0.001f * accum;
        cppinv->update_and_draw(delta);
    }
}
