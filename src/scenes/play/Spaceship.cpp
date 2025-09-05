#include "Spaceship.hpp"
#include "CppInvaders.hpp"

#define STARTING_X 16
#define X_BORDER 24
#define Y 216
#define VX 80

#define TIME_DEAD 1.5f
#define TIME_EXPLOSION_FRAME 0.1f

Spaceship::Spaceship() {
    state = DEPLOYING;
    x = STARTING_X;
    timer = 0;
}

void Spaceship::explode() {
    pico_output_sound(SFX_SPACESHIP_KILLED);
    state = EXPLODING;
    timer = 0;
    explosion_frames = 0;
}

// Shot *Spaceship::shoot() {
//     pico_output_sound(SFX_SPACESHIP_SHOOT);
//     Shot *shot = new Shot;
//     shot->state = Shot::ALIVE;
//     shot->x = x + 8;
//     shot->y = Y;
//     shot->vy = -200;
//     return shot;
// }

void Spaceship::update(float delta) {
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);
    Pico_Dim size = pico_get_size().log;

    switch (state) {
    case DEPLOYING:
        timer += delta;
        if (timer >= 2) {
            state = DEPLOYED;
            x = X_BORDER;
        }
        break;
    case DEPLOYED:
        x -= VX * delta * (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]);
        x += VX * delta * (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
        x = SDL_max(X_BORDER, SDL_min(size.x - X_BORDER, x));
        break;
    case EXPLODING:
        timer += delta;
        if (timer >= TIME_EXPLOSION_FRAME) {
            timer = 0;
            if (++explosion_frames >= 10) {
                state = DEPLOYING;
                timer = 0;
                CppInvaders::get().lives--;
            }
        }
        break;
    }
}

void Spaceship::draw() const {
    Pico_Pos pos = {(int)round(x), Y};

    pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    switch (state) {
    case DEPLOYING:
        break;
    case DEPLOYED:
        pico_set_crop({0, 0, 16, 8});
        pico_output_draw_image(pos, IMG_SPACESHIP);
        break;
    case EXPLODING:
        pico_set_crop({16 * (1 + explosion_frames % 2), 0, 16, 8});
        pico_output_draw_image(pos, IMG_SPACESHIP);
        break;
    }
}
