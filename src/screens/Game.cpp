#include "Screens.h"
#include "entts/Entities.h"

void CppInvaders::Game::process_collisions() {
    SDL_FRect ufo_rect = { ufo->x + 4, 40, 16, 8 },
              spaceship_rect = { spaceship->x, 216, 15, 8 },
              horde_shot_rect = { horde_shot->x - 1, horde_shot->y, 3, 4 },
              spaceship_shot_rect = { spaceship_shot->x, spaceship_shot->y, 1, 4 };

    // between shots
    if (horde_shot->state == Shot::ALIVE && spaceship_shot->state == Shot::ALIVE &&
        SDL_HasIntersectionF(&horde_shot_rect, &spaceship_shot_rect))
    {
        if (rand() % 2) {
            horde_shot->explode(0.3);
        }

        if (rand() % 2) {
            spaceship_shot->explode(0.3);
        }
    }

    // horde shot and spaceship
    if (horde_shot->state == Shot::ALIVE && spaceship->state == Spaceship::DEPLOYED &&
        SDL_HasIntersectionF(&horde_shot_rect, &spaceship_rect))
    {
        horde_shot->explode_without_img(0.3);
        spaceship->explode();
    }

    // spaceship shot and ufo
    if (ufo->state == UFO::ALIVE && spaceship_shot->state == Shot::ALIVE &&
        SDL_HasIntersectionF(&ufo_rect, &spaceship_shot_rect))
    {
        ufo->explode();
        spaceship_shot->explode_without_img(0.3);
    }

    // horde and other stuff
    for (int i = 0; i < 55; i++) {
        Invader& inv = horde->invaders[i];
        if (inv.state == Invader::DEAD) {
            continue;
        }

        SDL_Rect tmp = inv.get_rect();
        SDL_FRect inv_rect = { tmp.x, tmp.y, tmp.w, tmp.h };

        // invaderr and spaceship shot
        if (spaceship_shot->state == Shot::ALIVE &&
            SDL_HasIntersectionF(&spaceship_shot_rect, &inv_rect))
        {
            spaceship_shot->explode_without_img(0.3);
            horde->explode_invader(i);
            break;
        }

        // invader and spaceship (game over)
        if (spaceship->state == Spaceship::DEPLOYED && inv.y >= 216) {
            spaceship->explode();
            horde->explode_invader(i);
            break;
        }
    }

    // horde shot and bunkers

    // horde and bunkers
}

CppInvaders::Game::Game() {
    state = STARTING;
    ufo = new UFO;
    horde = new Horde;
    spaceship = new Spaceship;
    horde_shot = new Shot;
    spaceship_shot = new Shot;
    // time = 0;
    // cannon.lives = cannon_lives;
}

CppInvaders::Game::~Game() {
    delete ufo;
    delete horde;
    delete spaceship;
    delete horde_shot;
    delete spaceship_shot;
}

void CppInvaders::Game::draw() {
    cppinv->draw_scoreboard();
    cppinv->draw_credit_counter();

    ufo->draw();
    horde->draw();
    spaceship->draw();

    horde_shot->draw();
    spaceship_shot->draw();

    pico_set_color_draw(GREEN);
    pico_output_draw_line({ 0, 239 }, { 224, 239 });

    static char lives_text[12];
    sprintf(lives_text, "%1d", SDL_max(spaceship->lives, 0));

    pico_set_color_draw(WHITE);
    pico_output_draw_text({ 8, 240 }, lives_text);

    pico_set_image_crop({ 0, 0, 16, 8 });
    for (int i = 0; i < spaceship->lives - 1; i++) {
        pico_output_draw_image({ 24 + 16 * i, 240 }, IMG_SPACESHIP);
    }
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
    case STARTING:
        horde->update(delta);
        spaceship->update(delta);
        if (spaceship->state == Spaceship::DEPLOYED) {
            state = PLAYING;
            time = 0;
        }
        break;
    case PLAYING:
        time += delta;
        if (time >= 1 && spaceship->state == Spaceship::DEPLOYED) {
            if (horde_shot->state == Shot::DEAD) {
                Shot *shot = horde->shoot(spaceship->x);
                delete horde_shot;
                horde_shot = shot;
            }
        }

        horde->update(delta);
        ufo->update(delta);
        spaceship->update(delta);

        if (spaceship->state == Spaceship::EXPLODING) {
            state = RESTARTING;
            time = 0;
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);

        process_collisions();


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
    case RESTARTING:
        ufo->update(delta);
        if (horde->state == Horde::FROZEN) {
            horde->update(delta);
        }

        spaceship->update(delta);
        if (spaceship->state == Spaceship::DEPLOYING && !spaceship->lives) {
            cppinv->screen = OVER;
            cppinv->over = new Over;
        }
        else if (spaceship->state == Spaceship::DEPLOYED) {
            state = PLAYING;
            time = 0;
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);

        process_collisions();
        break;
    }
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
        break;
    }
}
