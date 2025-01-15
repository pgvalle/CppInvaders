#include "Horde.h"
#include "screens/Game.h"

std::vector<int> Horde::get_alive_invaders() {
    std::vector<int> alive_invaders;
    for (int i = 0; i < 55; i++) {
        if (invaders[i].state != Invader::DEAD) {
            alive_invaders.push_back(i);
        }
    }

    return alive_invaders;
}

void Horde::explode_invader(int index) {
    pico_assert(state == MARCHING);

    state = FROZEN;
    index_dying_invader = index;
    time = 0;

    int value = 10 * (3 - invaders[index_dying_invader].type);
    invaders[index_dying_invader].state = Invader::DEAD;
    cppinv->add_to_score(value);
}

Shot *Horde::shoot(float spaceship_x) {
    pico_assert(state == MARCHING);

    // choose a random invader from alive invaders
    std::vector<int> alive_invaders = get_alive_invaders();
    int r = rand() % alive_invaders.size();
    r = alive_invaders[r];

    SDL_Point rand_xy = { invaders[r].x, invaders[r].y },
              best_xy = { -1000, -1000 };

    for (Invader& inv : invaders) {
        if (inv.state == Invader::DEAD) {
            continue;
        }

        bool best_above_spaceship = (abs(spaceship_x - inv.x) <= 5);
        bool best_lower_y = inv.y > best_xy.y;

        if (best_above_spaceship && best_lower_y) {
            best_xy = { inv.x, inv.y };
        }
        
        bool rand_same_x = abs(rand_xy.x - inv.x) <= 3;
        bool rand_lower_y = inv.y >= rand_xy.y;
    
        if (rand_same_x && rand_lower_y) {
            rand_xy = { inv.x, inv.y };
        }
    }

    bool invalid_best_xy = (best_xy.x < 0 && best_xy.y < 0);
    bool random_shot = (rand() % 3 > 0);

    if (invalid_best_xy || random_shot) {
        best_xy = rand_xy;
    }

    Shot *shot = new Shot;
    shot->state = Shot::ALIVE;
    shot->x = best_xy.x + 5;
    shot->y = best_xy.y + 8;
    shot->vy = 120;
    return shot;
}

void Horde::draw() {
    for (Invader& inv : invaders) {
        inv.draw();
    }

    if (state == FROZEN) {
        Invader &inv = invaders[index_dying_invader];
        pico_set_image_crop({ 0, 0, 13, 8 });
        pico_output_draw_image({ inv.x, inv.y }, IMG_EXP1);
    }
}

void Horde::update(float delta) {
    switch (state) {
    case DEPLOYING:
        invaders[i].deploy(i);
        if (++i % 55 == 0) {
            state = MARCHING;
            i = 0;
            dx = 2;
            dy = 0;
            time = 0;
        }
        break;
    case MARCHING: { // TODO: Fix invaader wrong placement when changing direction
        bool cycle_complete = false;
        int j = i;
        while (invaders[j].state == Invader::DEAD) {
            j = (j + 1) % 55;
        }

        invaders[j].move(dx, dy);
        j = (j + 1) % 55;
        cycle_complete = j <= i;
        i = j;
    
        if (!cycle_complete) { // Check for direction change when all invaders are updated
            break;
        }

        if (dy != 0) { // All invaders updated when they were changing directions
            dy = 0;
            break;
        }

        for (Invader& inv : invaders) { // all invaders updated and direction may change
            bool out_of_bounds = (inv.x < 12 || inv.x > 200);
            if (inv.state != Invader::DEAD && out_of_bounds) {
                dx = -dx;
                dy = 8;
                break;
            }
        }
        break; }
    case FROZEN:
        time += delta;
        if (time >= 0.3) {
            if (get_alive_invaders().size() == 0) {
                state = DEPLOYING;
                i = 0;
            }
            else {
                state = MARCHING;
            }
        }
        break;
    }
}
