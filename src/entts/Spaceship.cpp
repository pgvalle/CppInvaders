#include "Spaceship.h"
#include "screens/Game.h"

#define STARTING_X 16
#define Y 216
#define VX 80

#define TIME_DEAD 1.5f
#define TIME_EXPLOSION_FRAME 0.1f

void Spaceship::explode() {
    pico_assert(state == DEPLOYED);

    pico_output_sound(SFX_SPACESHIP_KILLED);
    state = EXPLODING;
    timer = 0;
    explosion_frames = 0;
}

Shot *Spaceship::shoot() {
    pico_assert(state == DEPLOYED);

    pico_output_sound(SFX_SPACESHIP_SHOOT);
    Shot *shot = new Shot;
    shot->state = Shot::ALIVE;
    shot->x = x + 8;
    shot->y = Y;
    shot->vy = -200;
    return shot;
}

void Spaceship::draw() {
    int rx = round(x);

    switch (state) {
    case DEPLOYING:
        break;
    case DEPLOYED:
        pico_set_image_crop({ 0, 0, 16, 8 });
        pico_output_draw_image({ rx, Y }, IMG_SPACESHIP);
        break;
    case EXPLODING:
        pico_set_image_crop({ 16 * (1 + explosion_frames % 2), 0, 16, 8 });
        pico_output_draw_image({ rx, Y }, IMG_SPACESHIP);
        break;
    }
}

void Spaceship::update(float delta) {
    const Uint8 *keys = SDL_GetKeyboardState(nullptr);

    switch (state) {
    case DEPLOYING:
        timer += delta;
        if (timer >= 2) {
            state = DEPLOYED;
            x = STARTING_X;
        }
        break;
    case DEPLOYED:
        x -= VX * delta * (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]);
        x += VX * delta * (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);
        x = SDL_max(16, SDL_min(190, x));
        break;
    case EXPLODING:
        timer += delta;
        if (timer >= TIME_EXPLOSION_FRAME) {
            timer = 0;
            if (++explosion_frames >= 10) {
                state = DEPLOYING;
                timer = 0;
                lives--;
            }
        }
        break;
    }
}
