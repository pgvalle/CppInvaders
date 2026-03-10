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

bool Spaceship::collide_rect(Pico_Abs_Rect rct, Pico_Anchor anc) const {
    Pico_Rel_Rect ship_rct = { '!', {(float)SDL_roundf(x), (float)Y, 15, 8}, PICO_ANCHOR_N, NULL };
    Pico_Rel_Rect other_rct = { '!', {(float)rct.x, (float)rct.y, (float)rct.w, (float)rct.h}, anc, NULL };
    return state == DEPLOYED && pico_vs_rect_rect(&ship_rct, &other_rct);
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
    Pico_Abs_Dim size;
    pico_get_view(NULL, &size, NULL, NULL, NULL, NULL, NULL, NULL);
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
        x = SDL_max(X_BORDER, SDL_min(size.w - X_BORDER, x));
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
    if (state == DEPLOYING) return;

    float draw_x = (float)SDL_roundf(x);
    float draw_y = (float)Y;
    
    char key[256];
    Pico_Rel_Rect crop = { '!', {0, 0, 16, 8}, PICO_ANCHOR_NW, NULL };

    if (state == DEPLOYED) {
        snprintf(key, sizeof(key), "/crop/ship/deployed");
        crop.x = 0;
    } else if (state == EXPLODING) {
        snprintf(key, sizeof(key), "/crop/ship/exploding/%d", explosion_frames % 2);
        crop.x = 16.0f * (1 + explosion_frames % 2);
    }

    pico_layer_image_mode('=', "ship", IMG_SPACESHIP);
    pico_layer_sub_mode('=', key, "ship", &crop);
    Pico_Rel_Rect dst = { '!', {draw_x, draw_y, 16, 8}, PICO_ANCHOR_N, NULL };
    pico_output_draw_layer(key, &dst);
}
