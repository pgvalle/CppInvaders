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

