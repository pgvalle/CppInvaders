#include "Spaceship.h"
#include "screens/Game.h"

#define STARTING_X 16
#define Y 216
#define VX 80

#define TIME_DEAD 1.5f
#define TIME_EXPLOSION_FRAME 0.1f

void Spaceship::explode() {
    pico_assert(state == DEPLOYED);

    state = EXPLODING;
    time = 0;
    explosion_frames = 0;
}

Shot *Spaceship::shoot() {
    // assert(state == ALIVE);

    // time1 = 0;
    // time2 = 0;
    // e_frame = 0;

    // Shot *shot = new Shot;
    // shot->x = x + 7;
    // shot->y = Y;
    // shot->vy = -240;
    // return shot;
    return nullptr;
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
    // Shot *shot = GAMEVAR->spaceship_shot;

    switch (state) {
    case DEPLOYING:
        time += delta;
        if (time >= 2) {
            state = DEPLOYED;
            x = STARTING_X;
        }
        break;
    case DEPLOYED:
        x -= VX * delta * (keys[SDL_SCANCODE_A] || keys[SDL_SCANCODE_LEFT]);
        x += VX * delta * (keys[SDL_SCANCODE_D] || keys[SDL_SCANCODE_RIGHT]);

        // if (keys[SDL_SCANCODE_SPACE] && shot->state == Shot::DEAD) {
        //     delete shot;
        //     shot = new Shot;
        //     shot->state = Shot::ALIVE;
        //     shot->x = x + 8;
        //     shot->y = Y;
        //     shot->vy = -200;
        //     GAMEVAR->spaceship_shot = shot;
        // }
        break;
    case EXPLODING:
        time += delta;
        if (time >= TIME_EXPLOSION_FRAME) {
            time = 0;
            if (++explosion_frames >= 20) {
                state = DEPLOYING;
                time = 0;
                lives--;
            }
        }
        break;
    }
}
