#include "Horde.h"

int GAMESCOPE::Horde::count_alive_invaders() {
    int count = 0;
    for (int i = 0; i < 55; i++) {
        if (invaders[i].state != Invader::DEAD) {
            count++;
        }
    }
    return count;
}

void GAMESCOPE::Horde::explode_invader(int index) {
    state = FROZEN;
    index_dying_invader = index;
    time = 0;

    int value = 100 * (invaders[index_dying_invader].type + 1);
    invaders[index_dying_invader].state = Invader::DEAD;
    cppinv->add_to_score(value);
}

void GAMESCOPE::Horde::draw() {
    for (Invader& inv : invaders) {
        inv.draw();
    }

    if (state == FROZEN) {
        Invader &inv = invaders[index_dying_invader];
        pico_set_image_crop({ 0, 0, 13, 8 });
        pico_output_draw_image({ inv.x, inv.y }, IMG_EXP1);
    }
}

void GAMESCOPE::Horde::update(float delta) {
    switch (state) {
    case DEPLOYING:
        invaders[i].deploy(i);
        if (++i % 55 == 0) {
            state = MARCHING;
            i = 0;
            dx = 2;
            dy = 0;
        }
        break;
    case MARCHING: { // TODO: Fix invaader wrong placement when changing direction
        GAMEVAR;
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
            if (count_alive_invaders() == 0) {
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

// Shot *GAMESCOPE::Horde::shoot(float target_x)
// {
//     shoottime = 0;
//     shoottimer = rand() % 2 + 1;

//     float prob = (float)rand() / RAND_MAX;
//     int x, y;

//     if (prob > 0.5f)
//     {
//         int i = rand() % invaders.size();
//         x = invaders[i].x;
//         y = invaders[i].y;

//         for (const Invader &inv : invaders)
//         {
//             if (abs(inv.x - x) < 4 && inv.y > y)
//                 y = inv.y;
//         }
//     }
//     else
//     {
//         x = -10000;
//         y = -1;
//         int dx = abs(target_x - x);

//         for (const Invader &inv : invaders)
//         {
//             int new_dx = abs(inv.x - x);

//             if (new_dx < dx && inv.y > y)
//             {
//                 dx = new_dx;
//                 x = inv.x;
//                 y = inv.y;
//             }
//         }
//     }

//     Shot *shot = new Shot;
//     shot->x = x + 5;
//     shot->y = y + 8;
//     shot->vy = 90;
//     return shot;
// }

// Explosion *GAMESCOPE::Horde::collidedWithShot(Shot *shot)
// {
//     SDL_Rect shotRect = shot->getRect();

//     for (int j = 0; j < invaders.size(); j++)
//     {
//         SDL_Rect invRect = invaders[j].getRect();

//         if (SDL_HasIntersection(&shotRect, &invRect))
//         {
//             Explosion *exp = new Explosion;
//             exp->x = invaders[j].x;
//             exp->y = invaders[j].y;
//             exp->lifespan = 0.2;
//             exp->src = {0, 0, 13, 8};
//             exp->tex = g->tex_inv_death;

//             g->score += (3 - invaders[j].type) * 10;

//             invaders.erase(invaders.begin() + j);
//             if (i > j && invaders.size() > 0)
//                 i--;

//             return exp;
//         }
//     }

//     return nullptr;
// }

// bool GAMESCOPE::Horde::hasReachedCannon() const
// {
//     for (const Invader &inv : invaders)
//     {
//         if (inv.y >= Cannon::Y)
//             return true;
//     }

//     return false;
// }
