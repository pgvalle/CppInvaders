#include "Horde.hpp"
#include "Bullet.hpp"
#include <ranges>
#include <CppInvaders.hpp>

#define TIME_STEP 0.016f
#define TIME_FROZEN 1.0f

static const char* sfx[] = {
    SFX_INVADER_STEP0, SFX_INVADER_STEP1,
    SFX_INVADER_STEP2, SFX_INVADER_STEP3};

Horde::Horde() {
    state = DEPLOYING;
    invaders_alive = 0;
    timer = 0;
    i = 0;
}

int Horde::collide_rect(Pico_Abs_Rect rct, Pico_Anchor anc) const {
    Pico_Rel_Rect other_rct = { '!', {(float)rct.x, (float)rct.y, (float)rct.w, (float)rct.h}, anc, NULL };
    for (int i = 0; i < 55; i++) {
        if (!invaders[i].is_alive()) continue;
        Pico_Abs_Rect ir = invaders[i].get_rect();
        Pico_Rel_Rect inv_rct = { '!', {(float)ir.x, (float)ir.y, (float)ir.w, (float)ir.h}, PICO_ANCHOR_N, NULL };
        if (pico_vs_rect_rect(&other_rct, &inv_rct)) {
            return i;
        }
    }

    return -1;
}

int Horde::kill_invader(int i) {
    pico_output_sound(SFX_INVADER_KILLED);
    invaders[i].kill();
    invaders_alive--;

    int score = 10 * (3 - invaders[i].type);
    return score;
}

Bullet* Horde::shoot(float ship_x) {
    Pico_Abs_Pos pos = {0, 0};

    for (const Invader& inv : invaders) {
        bool above_ship = abs(ship_x - inv.x) <= 8;
        if (inv.y > pos.y && above_ship && inv.is_alive()) {
            pos = {inv.x, inv.y};
        }
    }

    if (rand() % 3 && pos.y > 0) {
        return new Bullet((float)pos.x, (float)pos.y + 12, 120);
    }

    std::vector<int> a;
    for (int i = 54; i >= 33; i--) {
        if (invaders[i].is_alive()) {
            a.push_back(i);
        }
    }
    int ri = rand() % a.size(); // FIXME: crashes when horde is empty
    pos = {invaders[a[ri]].x, 0};

    for (const Invader& inv : invaders | std::views::reverse) {
        bool same_column = abs(inv.x - pos.x) <= 8;
        if (inv.y > pos.y && same_column && inv.is_alive()) {
            pos = {inv.x, inv.y};
        }
    }

    return new Bullet((float)pos.x, (float)pos.y + 12, 120);
}

void Horde::update(float delta) {
    Pico_Abs_Dim size;
    pico_get_view(NULL, &size, NULL, NULL, NULL, NULL, NULL, NULL);
    timer += delta;

    switch (state) {
    case DEPLOYING:
        while (timer >= TIME_STEP && i < 55) {
            invaders[i] = Invader(i); i++;
            invaders_alive++;
            timer -= TIME_STEP;
            if (i == 55) {
                state = MARCHING;
                i = 0;
                sfx_i = 0;
                sfx_timer = 0;
                Invader::dx = 2;
                Invader::dy = 0;
                break;
            }
        }
        break;
    case MARCHING:
        for (Invader& inv : invaders) {
            if (inv.state == Invader::DYING) {
                inv.update(delta);
            }
        }

        while (timer >= TIME_STEP) {
            for (; i < 55 && !invaders[i].is_alive(); i++);

            if (i == 55) {
                for (i = 0; i < 55 && !invaders[i].is_alive(); i++);

                Invader::dy = 0;
                for (const Invader& inv : invaders) {
                    bool out = inv.x < 16 || inv.x >= size.w - 16;
                    if (inv.is_alive() && out) {
                        Invader::dx = -Invader::dx;
                        Invader::dy = 8;
                        break;
                    }
                }
            }

            if (i < 55) {
                invaders[i].update(delta); i++;
            }

            timer -= TIME_STEP;
        }

        sfx_timer += delta;
        if (sfx_timer >= TIME_STEP * invaders_alive + 0.1f) {
            pico_output_sound(sfx[sfx_i]);
            sfx_i = (sfx_i + 1) % 4;
            sfx_timer = 0;
        }
        break;
    }
}

void Horde::draw() const {
    for (const Invader& inv : invaders) {
        inv.draw();
    }
}
