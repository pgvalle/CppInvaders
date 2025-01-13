#include "Screens.h"
#include "entts/Entities.h"

// void CppInvaders::Game::process_collisions(float delta)
// {
//     for (Bunker &bun : bunkers)
//         bun.collideWithHorde(horde);

//     horde.shoottime += delta;
//     if (horde.shoottime > horde.shoottimer && !horde.invaders.empty())
//         shots.push_back(horde.shoot(cannon.x));

//     for (int i = 0; i < shots.size(); i++)
//     {
//         shots[i]->update(delta);

//         bool out_of_bounds = shots[i]->y > 232 || shots[i]->y < UFO::Y - 6;
//         bool collided = ufo.collidedWithShot(shots[i]) ||
//                         (shots[i]->vy > 0 && cannon.collidedWithShot(shots[i]));

//         if (out_of_bounds || collided)
//         {
//             create_shot_explosion(shots[i]);
//             delete shots[i];
//             shots.erase(shots.begin() + i--);
//             continue;
//         }

//         if (shots[i]->vy < 0)
//         {
//             Explosion *exp = horde.collidedWithShot(shots[i]);
//             if (exp) {
//                 explosions.push_back(exp);
//                 delete shots[i];
//                 shots.erase(shots.begin() + i--);
//                 continue;
//             }
//         }

//         bool a = false;
//         for (Bunker &bun : bunkers)
//         {
//             if (bun.collidedWithShot(shots[i])) {
//                 create_shot_explosion(shots[i]);
//                 delete shots[i];
//                 shots.erase(shots.begin() + i--);
//                 a = true;
//                 break;
//             }
//         }

//         if (a) continue;

//         SDL_Rect shotRectI = shots[i]->getRect();
//         shotRectI.x -= 2;
//         shotRectI.w += 4;

//         for (int j = i + 1; j < shots.size(); j++)
//         {
//             SDL_Rect shotRectJ = shots[j]->getRect();

//             if (!SDL_HasIntersection(&shotRectI, &shotRectJ))
//                 continue;
            
//             switch (rand() % 3)
//             {
//             case 0:
//                 create_shot_explosion(shots[j]);
//                 create_shot_explosion(shots[i]);
//                 break;

//             case 1:
//                 create_shot_explosion(shots[i]);
//                 break;

//             case 2:
//                 create_shot_explosion(shots[j]);
//                 break;
//             }

//             delete shots[j];
//             shots.erase(shots.begin() + j);
//             delete shots[i];
//             shots.erase(shots.begin() + i--);
//             break;
//         }
//     }
// }

CppInvaders::Game::Game() {
    state = POPULATING_HORDE;
    ufo = new UFO;
    spaceship = new Spaceship;
    horde_shot = new Shot;
    spaceship_shot = new Shot;
    // time = 0;
    // cannon.lives = cannon_lives;
}

CppInvaders::Game::~Game() {
    // for (Shot *shot : shots)
    //     delete shot;
    
    // for (Explosion *explosion : explosions)
    //     delete explosion;
}

void CppInvaders::Game::draw() {
    cppinv->draw_scoreboard();
    cppinv->draw_credit_counter();

    ufo->draw();
    spaceship->draw();

    horde_shot->draw();
    spaceship_shot->draw();
    // SDL_SetRenderDrawColor(app->renderer, 0, 0, 0, 255);
    // SDL_RenderClear(app->renderer);

    // ui_draw();

    // for (Shot *shot : shots)
    //     shot->draw();

    // for (Explosion *explosion : explosions)
    //     explosion->draw();

    // ufo.draw();
    // horde.draw();
    // cannon.draw();

    // for (auto &bun : bunkers)
    //     bun.draw();

    // cannon life counter drawn as backup cannons
    // for (int i = 0; i < cannon.lives - 1; i++)
    //     app->draw_clip(g->tex_cannon, 24 + 16 * i, 240, {0, 0, 16, 8});

    // cannon life counter
    // app->draw_text(g->font, 8, 240, "%d", cannon.lives);

    // SDL_SetRenderDrawColor(app->renderer, 0, 255, 0, 255);
    // SDL_RenderDrawLine(app->renderer, 0, 239, WIDTH, 239);
}

void CppInvaders::Game::update(float delta) {
    switch (state) {
    case POPULATING_HORDE:
        // horde.populate();
        // if (horde.invaders.size() == 55)
            state = PLAYING;
        break;

    case PLAYING: // player playing
        ufo->update(delta);
        spaceship->update(delta);

        horde_shot->update(delta);
        spaceship_shot->update(delta);

        // update_explosions(delta);
        // process_collisions(delta);
        // ufo.update(delta);
        // horde.move();
        // cannon.update(delta);

        // if (horde.hasReachedCannon())
        //     cannon.explode();

        // if (cannon.state == Cannon::EXPLODING)
        // {
        //     state = CANNON_EXPLODING;

        //     for (Shot *shot : shots)
        //         delete shot;
        //     shots.clear();
        // }

        // if (horde.invaders.empty())
        // {
        //     state = RESTARTING;

        //     for (Shot *shot : shots)
        //         delete shot;
        //     shots.clear();
        // }

        break;

    case CANNON_EXPLODING:
        // update_explosions(delta);
        // // cannon.update(delta);

        // if (cannon.state == Cannon::DEAD)
        //     state = CANNON_DEAD;

        // if (cannon.state == Cannon::DEAD && horde.hasReachedCannon())
        //     cannon.lives = 0;        

        // if (!cannon.lives)
        //     app->stack.push(new OverScreen(this));
        break;

    case CANNON_DEAD:
        // cannon.update(delta);
        // if (cannon.state == Cannon::ALIVE)
        //     state = PLAYING;
        break;

    case RESTARTING:
        // update_explosions(delta);
        // time += delta;
        // if (time > TIME_RESTARTING)
        //     app->stack.popThenPush(new Game(cannon.lives), 1);
        break;
    }

    // if (g->score > g->hi_score)
    //     g->hi_score = g->score;
}

void CppInvaders::Game::process_event(const SDL_Event &event) {
    switch (event.type) {
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym) {
        case SDLK_ESCAPE:
            cppinv->screen = PAUSE;
            cppinv->pause = new Pause;
            break;
        }
        // switch (event.key.keysym.sym)
        // {
        // case SDLK_SPACE:
        //     if (state == PLAYING && cannon.time1 >= 0.75)
        //         shots.push_back(cannon.shoot());
        //     break;

        // case SDLK_ESCAPE:
        //     app->stack.push(new PauseScreen(this));
        //     break;
        // }
        break;
    }
}
