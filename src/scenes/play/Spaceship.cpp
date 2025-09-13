#include "Spaceship.hpp"
#include "Bullet.hpp"
#include <CppInvaders.hpp>

#define X_START 16
#define X_BORDER 24
#define Y 216
#define VX 80

#define TIME_DEPLOYING 2.0f
#define TIME_DEAD 1.5f
#define TIME_EXPLOSION_FRAME 0.1f

Spaceship::Spaceship() {
    state = DEPLOYING;
    x = X_START;
    timer = 0;
    lives = 3;
}

bool Spaceship::collide_rect(Pico_Rect rct, Pico_Anchor anc) const {
    Pico_Rect ship_rct = {(int)SDL_roundf(x), Y, 15, 8};
    Pico_Anchor ship_anc = {PICO_CENTER, PICO_TOP};
    return state == DEPLOYED && pico_rect_vs_rect_ext(rct, anc, ship_rct, ship_anc);
}

void Spaceship::explode() {
    pico_output_sound(SFX_SPACESHIP_KILLED);
    state = EXPLODING;
    timer = 0;
    explosion_frames = 0;
}

Bullet* Spaceship::shoot() {
    pico_output_sound(SFX_SPACESHIP_SHOOT);
    return new Bullet(x, Y, -200);
}

void Spaceship::update(float delta) {
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);
    Pico_Dim size = pico_get_size().log;
    timer += delta;

    switch (state) {
    case DEPLOYING:
        if (timer >= TIME_DEPLOYING) {
            state = DEPLOYED;
            x = X_BORDER;
            timer -= TIME_DEPLOYING;
        }
        break;
    case DEPLOYED:
        x -= VX * delta * (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]);
        x += VX * delta * (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
        x = SDL_max(X_BORDER, SDL_min(size.x - X_BORDER, x));
        timer -= delta;
        break;
    case EXPLODING:
        if (timer >= TIME_EXPLOSION_FRAME) {
            timer -= TIME_EXPLOSION_FRAME;
            if (++explosion_frames >= 10) {
                state = DEPLOYING;
                lives--;
            }
        }
        break;
    }
}

void Spaceship::draw() const {
    Pico_Pos pos = {(int)SDL_roundf(x), Y};

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
