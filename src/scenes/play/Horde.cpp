#include "Horde.hpp"
#include "Bullet.hpp"
#include <CppInvaders.hpp>

#define TIME_STEP 0.016f

static const char* sfx[] = {
    SFX_INVADER_STEP0, SFX_INVADER_STEP1,
    SFX_INVADER_STEP2, SFX_INVADER_STEP3};

Horde::Horde() {
    state = PREDEPLOY;
    timer = 0;
}

void Horde::explode_invader(int index) {
    pico_output_sound(SFX_INVADER_KILLED);
    state = FROZEN;
    index_dying_invader = index;
    timer = 0;

    int value = 10 * (3 - invaders[index_dying_invader].type);
    // cppinv->add_to_score(value);

    invaders[index_dying_invader].state = Invader::DEAD;
}

Bullet* Horde::shoot(float ship_x) {
    const Invader &inv = invaders[rand() % 55];
    Pico_Pos pos = {0, 0};
    Pico_Pos rnd_pos = {inv.x, inv.y};

    for (const Invader& inv : invaders) {
        if (inv.state == Invader::DEAD) {
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

    while (timer >= TIME_STEP) {
        switch (state) {
        case PREDEPLOY:
            state = DEPLOYING;
            break;
        case DEPLOYING:
            invaders[i++].state = Invader::DOWN;
            if (i == 55) {
                state = MARCHING;
                i = 0;
                dx = 2;
                dy = 0;
                sfx_i = 0;
                sfx_timer = 0;
            }
            break;
        case MARCHING:
            for (; i < 55 && invaders[i].state == Invader::DEAD; i++);

            if (i == 55) {
                for (i = 0; i < 55 && invaders[i].state == Invader::DEAD; i++);

                dy = 0;
                for (const Invader& inv : invaders) {
                    bool out_of_bounds = (inv.x < 16 || inv.x >= size.x - 16);
                    if (out_of_bounds && inv.state != Invader::DEAD) {
                        dx = -dx;
                        dy = 8;
                        break;
                    }
                }
            }

            if (i < 55) {
                invaders[i++].move(dx, dy);
            }

            break;
        case FROZEN:
            state = MARCHING;
            break;
        }

        timer -= TIME_STEP;
    }

    if (state == MARCHING) {
        sfx_timer += delta;
        if (sfx_timer >= 1) {
            pico_output_sound(sfx[sfx_i]);
            sfx_i = (sfx_i + 1) % 4;
            sfx_timer = 0;
        }
    }
}

void Horde::draw() const {
    for (const Invader& inv : invaders) {
        inv.draw();
    }

    // pico_set_anchor_draw({PICO_CENTER, PICO_TOP});
    // if (state == FROZEN) {
    //     Invader &inv = invaders[index_dying_invader];
    //     pico_output_draw_image({ inv.x, inv.y }, IMG_EXP1);
    // }
}
