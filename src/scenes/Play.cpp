#include "Play.hpp"
#include "Pause.hpp"
#include "Over.hpp"
#include "CppInvaders.hpp"

PlayScene::PlayScene() {
    CppInvaders::get().lives = 3;
    horde_b = nullptr;
    ship_b = nullptr;
}

PlayScene::~PlayScene() {
    delete horde_b;
    delete ship_b;
}

void PlayScene::process_event(const Pico_Event &event) {
    switch (event.type) {
    case PICO_KEYDOWN:
        switch (event.key.keysym.sym) {
        case PICOK_SPACE:
            if (horde.state == Horde::MARCHING && ship.state == Spaceship::DEPLOYED && !ship_b) {
                ship_b = ship.shoot();
            }
            break;
        case PICOK_ESCAPE:
            CppInvaders::get().scene = new PauseScene(this);
            break;
        }
        break;
    default:
        break;
    }
}

void PlayScene::process_collisions() {
    Pico_Anchor b_anc = {PICO_CENTER, PICO_MIDDLE};
    if (ship_b && ship_b->state == Bullet::ALIVE) {
        Pico_Rect ship_b_rct = ship_b->get_rect();

        if (ufo.collide_rect(ship_b_rct, b_anc)) {
            ufo.explode();
            ship_b->die(0.3);
        }

        int i = horde.collide_rect(ship_b_rct, b_anc);
        if (i >= 0) {
            horde.kill_invader(i);
            ship_b->die(0.3);
        }

        // TODO: bunker collision with ship bullet
    }

    if (horde_b && horde_b->state == Bullet::ALIVE) {
        Pico_Rect horde_b_rct = horde_b->get_rect();
        if (ship.collide_rect(horde_b_rct, b_anc)) {
            ship.explode();
            horde_b->die(0.3);
        }
        
        // TODO: bunker collision with horde bullet
    }

}

void PlayScene::update(float delta) {
    if (horde_b) {
        horde_b->update(delta);
        if (horde_b->state == Bullet::DEAD) {
            delete horde_b;
            horde_b = nullptr;
        }
    } else if (ship.state == Spaceship::DEPLOYED) {
        horde_b = horde.shoot(ship.x);
    }

    if (ship_b) {
        ship_b->update(delta);
        if (ship_b->state == Bullet::DEAD) {
            delete ship_b;
            ship_b = nullptr;
        }
    }

    process_collisions();

    ufo.update(delta);
    horde.update(delta);
    ship.update(delta);

    if (CppInvaders::get().lives == 0) {
        CppInvaders::get().scene = new OverScene(this);
    }
}

void PlayScene::draw() const {
    ufo.draw();
    horde.draw();
    if (horde_b) {
        horde_b->draw();
    }
    if (ship_b) {
        ship_b->draw();
    }
    ship.draw();

    Pico_Pos pos = pico_pos({0, 100});
    pos = {pos.x + 8, pos.y - 8};
    pico_set_anchor_draw({PICO_LEFT, PICO_BOTTOM});
    pico_set_crop({0, 0, 0, 0});
    pico_set_color_draw(WHITE);
    pico_output_draw_fmt(pos, "%1d", CppInvaders::get().lives);

    pico_set_crop({0, 0, 16, 8});
    for (int i = 1; i < CppInvaders::get().lives; i++) {
        pos.x += 16;
        pico_output_draw_image(pos, IMG_SPACESHIP);
    }
}

/*
void CppInvaders::Game::process_collisions() {
    SDL_Rect ufo_rect = { ROUND(ufo->x) + 4, 40, 16, 8 },
             spaceship_rect = { ROUND(spaceship->x), 216, 15, 8 },
             horde_shot_rect = { ROUND(horde_shot->x), ROUND(horde_shot->y), 1, 4 },
             spaceship_shot_rect = { ROUND(spaceship_shot->x), ROUND(spaceship_shot->y), 1, 4 };

    // between shots
    horde_shot_rect.x -= 1;
    horde_shot_rect.w = 3;
    
    if (horde_shot->state == Shot::ALIVE && spaceship_shot->state == Shot::ALIVE &&
        SDL_HasIntersection(&horde_shot_rect, &spaceship_shot_rect))
    {
        switch (rand() % 3) {
        case 0: // horde shot explodes
            horde_shot->explode(0.3);
            break;
        case 1: // spaceship shot explodes
            spaceship_shot->explode(0.3);
            break;
        case 2: // both explode
            horde_shot->explode(0.3);
            spaceship_shot->explode(0.3);
            break;
        }
    }

    horde_shot_rect.x += 1;
    horde_shot_rect.w = 1;

    // horde shot and spaceship
    if (horde_shot->state == Shot::ALIVE && spaceship->state == Spaceship::DEPLOYED &&
        SDL_HasIntersection(&horde_shot_rect, &spaceship_rect))
    {
        horde_shot->explode_without_img(0.3);
        spaceship->explode();
    }

    // spaceship shot and ufo
    if (ufo->state == UFO::ALIVE && spaceship_shot->state == Shot::ALIVE &&
        SDL_HasIntersection(&ufo_rect, &spaceship_shot_rect))
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

        SDL_Rect inv_rect = inv.get_rect();
        
        // invader and shields
        for (Shield &shield : shields) {
            shield.damage(inv_rect);
        }

        // invader and spaceship shot
        if (spaceship_shot->state == Shot::ALIVE &&
            SDL_HasIntersection(&spaceship_shot_rect, &inv_rect))
        {
            spaceship_shot->explode_without_img(0.3);
            horde->explode_invader(i);
        }

        // invader and spaceship (game over)
        if (spaceship->state == Spaceship::DEPLOYED && inv.y >= 216) {
            horde_reached_spaceship = true;
            spaceship->explode();
        }
    }

    // shield
    for (Shield &shield : shields) {
        SDL_Rect shield_rect = { shield.x, 192, 22, 16 };

        if (SDL_HasIntersection(&horde_shot_rect, &shield_rect) &&
            horde_shot->state == Shot::ALIVE && shield.damage(horde_shot))
        {
            horde_shot->explode(0.3);
        }

        if (SDL_HasIntersection(&spaceship_shot_rect, &shield_rect) &&
            spaceship_shot->state == Shot::ALIVE && shield.damage(spaceship_shot))
        {
            spaceship_shot->explode(0.3);
        }
    }

    // horde and bunkers
}

CppInvaders::Game::Game() {
    state = STARTING;
    horde_reached_spaceship = false;
    ufo = new UFO;
    horde = new Horde;
    spaceship = new Spaceship;
    horde_shot = new Shot;
    spaceship_shot = new Shot;

    int x = 32;
    for (Shield &shield : shields) {
        shield.deploy(x);
        x += 46;
    }
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

    for (Shield &shield : shields) {
        shield.draw();
    }

    spaceship_shot->draw();

    pico_set_color_draw(GREEN);
    pico_output_draw_line({ 0, 239 }, { 224, 239 });

    static char text[12];
    sprintf(text, "%1d", SDL_max(spaceship->lives, 0));

    pico_set_color_draw(WHITE);
    pico_output_draw_text({ 8, 240 }, text);

    pico_set_image_crop({ 0, 0, 16, 8 });
    for (int i = 0; i < spaceship->lives - 1; i++) {
        pico_output_draw_image({ 24 + 16 * i, 240 }, IMG_SPACESHIP);
    }
}

void CppInvaders::Game::update(float delta) {
    switch (state) {
    case STARTING:
        horde->update(delta);
        spaceship->update(delta);
        if (spaceship->state == Spaceship::DEPLOYED) {
            state = PLAYING;
            timer = 0;
        }
        break;
    case PLAYING:
        process_collisions();

        timer += delta;
        if (timer >= 1 && spaceship->state == Spaceship::DEPLOYED &&
            horde->state == Horde::MARCHING && horde_shot->state == Shot::DEAD)
        {
            Shot *shot = horde->shoot(spaceship->x);
            delete horde_shot;
            horde_shot = shot;
        }

        ufo->update(delta);
        horde->update(delta);
        if (horde->get_alive_invaders().size() == 0) {
            state = RESTARTING2;
            timer = 0;
        }

        spaceship->update(delta);
        if (spaceship->state == Spaceship::EXPLODING) {
            state = RESTARTING;
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);
        break;
    case RESTARTING:
        process_collisions();

        ufo->update(delta);
        if (horde->state == Horde::FROZEN) {
            horde->update(delta);
        }

        spaceship->update(delta);
        if (spaceship->state == Spaceship::DEPLOYING && (!spaceship->lives || horde_reached_spaceship)) {
            cppinv->screen = OVER;
            cppinv->over = new Over;
        }
        else if (spaceship->state == Spaceship::DEPLOYED) {
            state = PLAYING;
            timer = 0;
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);
        break;
    case RESTARTING2:
        if (horde->state == Horde::FROZEN) {
            horde->update(delta);
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);

        timer += delta;
        if (timer >= 1) {
            int lives = spaceship->lives;
            delete ufo;
            delete spaceship;
            delete horde_shot;
            delete spaceship_shot;

            ufo = new UFO;
            spaceship = new Spaceship;
            horde_shot = new Shot;
            spaceship_shot = new Shot;
            state = STARTING;
            spaceship->lives = lives;

            int x = 32;
            for (Shield &shield : shields) {
                shield.deploy(x);
                x += 46;
            }
        }
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
        case SDLK_SPACE:
            if (state == PLAYING && spaceship_shot->state == Shot::DEAD &&
                timer >= 1 && spaceship->state == Spaceship::DEPLOYED)
            {
                Shot *shot = spaceship->shoot();
                delete spaceship_shot;
                spaceship_shot = shot;
            }
            break;
        }
        break;
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
            cppinv->screen = PAUSE;
            cppinv->pause = new Pause;
        }
        break;
    }
}
*/
/*
void CppInvaders::Game::process_collisions() {
    SDL_Rect ufo_rect = { ROUND(ufo->x) + 4, 40, 16, 8 },
             spaceship_rect = { ROUND(spaceship->x), 216, 15, 8 },
             horde_shot_rect = { ROUND(horde_shot->x), ROUND(horde_shot->y), 1, 4 },
             spaceship_shot_rect = { ROUND(spaceship_shot->x), ROUND(spaceship_shot->y), 1, 4 };

    // between shots
    horde_shot_rect.x -= 1;
    horde_shot_rect.w = 3;
    
    if (horde_shot->state == Shot::ALIVE && spaceship_shot->state == Shot::ALIVE &&
        SDL_HasIntersection(&horde_shot_rect, &spaceship_shot_rect))
    {
        switch (rand() % 3) {
        case 0: // horde shot explodes
            horde_shot->explode(0.3);
            break;
        case 1: // spaceship shot explodes
            spaceship_shot->explode(0.3);
            break;
        case 2: // both explode
            horde_shot->explode(0.3);
            spaceship_shot->explode(0.3);
            break;
        }
    }

    horde_shot_rect.x += 1;
    horde_shot_rect.w = 1;

    // horde shot and spaceship
    if (horde_shot->state == Shot::ALIVE && spaceship->state == Spaceship::DEPLOYED &&
        SDL_HasIntersection(&horde_shot_rect, &spaceship_rect))
    {
        horde_shot->explode_without_img(0.3);
        spaceship->explode();
    }

    // spaceship shot and ufo
    if (ufo->state == UFO::ALIVE && spaceship_shot->state == Shot::ALIVE &&
        SDL_HasIntersection(&ufo_rect, &spaceship_shot_rect))
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

        SDL_Rect inv_rect = inv.get_rect();
        
        // invader and shields
        for (Shield &shield : shields) {
            shield.damage(inv_rect);
        }

        // invader and spaceship shot
        if (spaceship_shot->state == Shot::ALIVE &&
            SDL_HasIntersection(&spaceship_shot_rect, &inv_rect))
        {
            spaceship_shot->explode_without_img(0.3);
            horde->explode_invader(i);
        }

        // invader and spaceship (game over)
        if (spaceship->state == Spaceship::DEPLOYED && inv.y >= 216) {
            horde_reached_spaceship = true;
            spaceship->explode();
        }
    }

    // shield
    for (Shield &shield : shields) {
        SDL_Rect shield_rect = { shield.x, 192, 22, 16 };

        if (SDL_HasIntersection(&horde_shot_rect, &shield_rect) &&
            horde_shot->state == Shot::ALIVE && shield.damage(horde_shot))
        {
            horde_shot->explode(0.3);
        }

        if (SDL_HasIntersection(&spaceship_shot_rect, &shield_rect) &&
            spaceship_shot->state == Shot::ALIVE && shield.damage(spaceship_shot))
        {
            spaceship_shot->explode(0.3);
        }
    }

    // horde and bunkers
}

CppInvaders::Game::Game() {
    state = STARTING;
    horde_reached_spaceship = false;
    ufo = new UFO;
    horde = new Horde;
    spaceship = new Spaceship;
    horde_shot = new Shot;
    spaceship_shot = new Shot;

    int x = 32;
    for (Shield &shield : shields) {
        shield.deploy(x);
        x += 46;
    }
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

    for (Shield &shield : shields) {
        shield.draw();
    }

    spaceship_shot->draw();

    pico_set_color_draw(GREEN);
    pico_output_draw_line({ 0, 239 }, { 224, 239 });

    static char text[12];
    sprintf(text, "%1d", SDL_max(spaceship->lives, 0));

    pico_set_color_draw(WHITE);
    pico_output_draw_text({ 8, 240 }, text);

    pico_set_image_crop({ 0, 0, 16, 8 });
    for (int i = 0; i < spaceship->lives - 1; i++) {
        pico_output_draw_image({ 24 + 16 * i, 240 }, IMG_SPACESHIP);
    }
}

void CppInvaders::Game::update(float delta) {
    switch (state) {
    case STARTING:
        horde->update(delta);
        spaceship->update(delta);
        if (spaceship->state == Spaceship::DEPLOYED) {
            state = PLAYING;
            timer = 0;
        }
        break;
    case PLAYING:
        process_collisions();

        timer += delta;
        if (timer >= 1 && spaceship->state == Spaceship::DEPLOYED &&
            horde->state == Horde::MARCHING && horde_shot->state == Shot::DEAD)
        {
            Shot *shot = horde->shoot(spaceship->x);
            delete horde_shot;
            horde_shot = shot;
        }

        ufo->update(delta);
        horde->update(delta);
        if (horde->get_alive_invaders().size() == 0) {
            state = RESTARTING2;
            timer = 0;
        }

        spaceship->update(delta);
        if (spaceship->state == Spaceship::EXPLODING) {
            state = RESTARTING;
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);
        break;
    case RESTARTING:
        process_collisions();

        ufo->update(delta);
        if (horde->state == Horde::FROZEN) {
            horde->update(delta);
        }

        spaceship->update(delta);
        if (spaceship->state == Spaceship::DEPLOYING && (!spaceship->lives || horde_reached_spaceship)) {
            cppinv->screen = OVER;
            cppinv->over = new Over;
        }
        else if (spaceship->state == Spaceship::DEPLOYED) {
            state = PLAYING;
            timer = 0;
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);
        break;
    case RESTARTING2:
        if (horde->state == Horde::FROZEN) {
            horde->update(delta);
        }

        horde_shot->update(delta);
        spaceship_shot->update(delta);

        timer += delta;
        if (timer >= 1) {
            int lives = spaceship->lives;
            delete ufo;
            delete spaceship;
            delete horde_shot;
            delete spaceship_shot;

            ufo = new UFO;
            spaceship = new Spaceship;
            horde_shot = new Shot;
            spaceship_shot = new Shot;
            state = STARTING;
            spaceship->lives = lives;

            int x = 32;
            for (Shield &shield : shields) {
                shield.deploy(x);
                x += 46;
            }
        }
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
        case SDLK_SPACE:
            if (state == PLAYING && spaceship_shot->state == Shot::DEAD &&
                timer >= 1 && spaceship->state == Spaceship::DEPLOYED)
            {
                Shot *shot = spaceship->shoot();
                delete spaceship_shot;
                spaceship_shot = shot;
            }
            break;
        }
        break;
    case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST) {
            cppinv->screen = PAUSE;
            cppinv->pause = new Pause;
        }
        break;
    }
}
*/
