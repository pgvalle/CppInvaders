#include "Horde.hpp"
#include "Bullet.hpp"
#include <CppInvaders.hpp>

#define TIME_STEP 0.016f
#define TIME_FROZEN 1.0f

static const char* sfx[] = {
    SFX_INVADER_STEP0, SFX_INVADER_STEP1,
    SFX_INVADER_STEP2, SFX_INVADER_STEP3};

Horde::Horde() {
    state = PREDEPLOY;
    timer = 0;
}

int Horde::collide_bullet(const Bullet* b) const {
    Pico_Rect b_rct = {b->x, b->y, 1, 7};
    for (int i = 0; i < 55; i++) {
        if (!invaders[i].is_alive()) {
            continue;
        }

        Pico_Rect inv_rct = invaders[i].get_rect();
        bool collided = pico_rect_vs_rect_ext(
                b_rct, {PICO_CENTER, PICO_MIDDLE},
                inv_rct, {PICO_CENTER, PICO_TOP});
        if (collided) {
            return i;
        }
    }

    return -1;
}

void Horde::kill_invader(int i$) {
    pico_output_sound(SFX_INVADER_KILLED);
    state = FROZEN;
    dying_inv_i = i$;
    timer = 0;

    // int value = 10 * (3 - invaders[dying_inv_i].type);
    // cppinv->add_to_score(value);

    invaders[i$].state = Invader::DYING;
}

Bullet* Horde::shoot(float ship_x) {
    const Invader &inv = invaders[rand() % 55];
    Pico_Pos pos = {0, 0};
    Pico_Pos rnd_pos = {inv.x, inv.y};

    for (const Invader& inv : invaders) {
        if (!inv.is_alive()) {
            continue;
        }

        bool inv_above_spaceship = abs(ship_x - inv.x) <= 8;
        if (inv.y > pos.y && inv_above_spaceship) {
            pos = {inv.x, inv.y};
        }
        
        bool good_rnd_x = abs(rnd_pos.x - inv.x) <= 8;
        if (inv.y > rnd_pos.y && good_rnd_x) {
            rnd_pos = {inv.x, inv.y};
        }
    }

    if (rand() % 3 == 0 || pos.y == 0) {
        pos = rnd_pos;
    }

    return new Bullet(pos.x, pos.y + 8, 120);
}

void Horde::update(float delta) {
    Pico_Dim size = pico_get_size().log;
    timer += delta;

    switch (state) {
    case PREDEPLOY:
        if (timer >= TIME_STEP) {
            state = DEPLOYING;
            i = 0;
            timer -= TIME_STEP;
        }
        break;
    case DEPLOYING:
        while (timer >= TIME_STEP) {
            invaders[i].state = Invader::DOWN;
            timer -= TIME_STEP;
            i++;
            if (i == 55) {
                state = MARCHING;
                i = 0;
                dx = 2;
                dy = 0;
                sfx_i = 0;
                sfx_timer = 0;
                break;
            }
        }
        break;
    case MARCHING:
        while (timer >= TIME_STEP) {
            for (; i < 55 && !invaders[i].is_alive(); i++) {
                if (invaders[i].state == Invader::DYING) {
                    invaders[i].state = Invader::DEAD;
                }
            }

            if (i == 55) {
                for (i = 0; i < 55 && !invaders[i].is_alive(); i++) {
                    if (invaders[i].state == Invader::DYING) {
                        invaders[i].state = Invader::DEAD;
                    }
                }

                dy = 0;
                for (const Invader& inv : invaders) {
                    bool out = inv.x < 16 || inv.x >= size.x - 16;
                    if (out && inv.is_alive()) {
                        dx = -dx;
                        dy = 8;
                        break;
                    }
                }
            }

            if (i < 55) {
                invaders[i].move(dx, dy);
                i++;
            }
            timer -= delta;
        }

        sfx_timer += delta;
        if (sfx_timer >= 1) {
            pico_output_sound(sfx[sfx_i]);
            sfx_i = (sfx_i + 1) % 4;
            sfx_timer = 0;
        }
        break;
    case FROZEN:
        if (timer >= TIME_FROZEN) {
            state = MARCHING;
            timer -= TIME_FROZEN;
        }
        break;
    }
}

void Horde::draw() const {
    for (const Invader& inv : invaders) {
        inv.draw();
    }

    // pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    // if (state == FROZEN) {
    //     Invader &inv = invaders[dying_inv_i];
    //     pico_output_draw_image({ inv.x, inv.y }, IMG_EXP1);
    // }
}
