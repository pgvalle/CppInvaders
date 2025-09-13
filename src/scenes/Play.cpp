#include "Play.hpp"
#include "Pause.hpp"
#include "Over.hpp"
#include "CppInvaders.hpp"

PlayScene::PlayScene() {
    state = STARTING;
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
            if (!ship_b && state == PLAYING && ship.state == Spaceship::DEPLOYED) {
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
            int score = horde.kill_invader(i);
            ship_b->die(0.3);
            CppInvaders::get().add_to_score(score);
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
    switch (state) {
    case STARTING:
        horde.update(delta);
        ship.update(delta);
        if (horde.state == Horde::MARCHING) {
            state = PLAYING;
        }
        break;
    case PLAYING:
        if (horde_b) {
            horde_b->update(delta);
            if (horde_b->state == Bullet::DEAD) {
                delete horde_b;
                horde_b = nullptr;
            }
        } else if (horde.invaders_alive > 0 && ship.state == Spaceship::DEPLOYED) {
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

        if (horde.invaders_alive == 0 || ship.lives == 0) {
            state = FIN;
            timer = 0;
            ufo = UFO();
            horde = Horde();
            delete horde_b;
            horde_b = nullptr;
            delete ship_b;
            ship_b = nullptr;
        }
        break;
    case FIN:
        timer += delta;
        if (timer >= 2 && ship.lives == 0) {
            CppInvaders::get().scene = new OverScene(this);
        } else if (timer >= 2) {
            state = STARTING;
            ship = Spaceship();
        }
        break;
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
    pos = {8, pos.y - 8};
    pico_set_anchor_draw({PICO_LEFT, PICO_BOTTOM});
    pico_set_crop({0, 0, 0, 0});
    pico_set_color_draw(WHITE);
    pico_output_draw_fmt(pos, "%1d", ship.lives);

    pico_set_crop({0, 0, 16, 8});
    for (int i = 1; i < ship.lives; i++) {
        pos.x += 16;
        pico_output_draw_image(pos, IMG_SPACESHIP);
    }
}

