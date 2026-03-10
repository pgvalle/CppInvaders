#include "Play.hpp"
#include "Pause.hpp"
#include "Over.hpp"
#include "CppInvaders.hpp"

PlayScene::PlayScene() {
    state = STARTING;
    horde_b = nullptr;
    ship_b = nullptr;

    for (int i = 0; i < 4; i++) {
        Shield s;
        s.deploy(32 + i * 48);
        shields.push_back(s);
    }
}

PlayScene::~PlayScene() {
    delete horde_b;
    delete ship_b;
}

void PlayScene::process_event(const Pico_Event &event) {
    switch (event.type) {
    case PICO_EVENT_KEY_DOWN:
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
    Pico_Anchor b_anc = PICO_ANCHOR_C;
    if (ship_b && ship_b->state == Bullet::ALIVE && horde_b && horde_b->state == Bullet::ALIVE) {
        Pico_Abs_Rect ship_b_rct = ship_b->get_rect();
        Pico_Abs_Rect horde_b_rct = horde_b->get_rect();
        if (SDL_HasIntersection(&ship_b_rct, &horde_b_rct)) {
            ship_b->explode(0.1);
            horde_b->explode(0.1);
        }
    }

    if (ship_b && ship_b->state == Bullet::ALIVE) {
        Pico_Abs_Rect ship_b_rct = ship_b->get_rect();

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

        for (auto &s : shields) {
            if (s.damage(ship_b)) {
                ship_b->explode(0.1);
                break;
            }
        }
    }

    if (horde_b && horde_b->state == Bullet::ALIVE) {
        Pico_Abs_Rect horde_b_rct = horde_b->get_rect();
        if (ship.collide_rect(horde_b_rct, b_anc)) {
            ship.explode();
            horde_b->die(0.3);
        }
        
        for (auto &s : shields) {
            if (s.damage(horde_b)) {
                horde_b->explode(0.1);
                break;
            }
        }
    }

    for (int i = 0; i < 55; i++) {
        if (horde.invaders[i].is_alive()) {
            Pico_Abs_Rect inv_rct = horde.invaders[i].get_rect();
            for (auto &s : shields) {
                s.damage(inv_rct);
            }
        }
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
            ufo = UFO();
            horde = Horde();
            ship = Spaceship();
            for (auto &s : shields) {
                s.deploy(s.x);
            }
        }
        break;
    }


}

void PlayScene::draw() const {
    ufo.draw();
    horde.draw();
    for (const auto &s : shields) {
        s.draw();
    }
    if (horde_b) {
        horde_b->draw();
    }
    if (ship_b) {
        ship_b->draw();
    }
    ship.draw();

    pico_set_color_draw(PICO_COLOR_WHITE);
    
    // Life counter at bottom-left
    char lives_buf[16];
    snprintf(lives_buf, sizeof(lives_buf), "%1d", ship.lives);
    Pico_Rel_Rect lives_txt_r = { '!', {8.0f, 248.0f, 0, 8}, PICO_ANCHOR_SW, NULL };
    pico_output_draw_text(lives_buf, &lives_txt_r);

    // Ship icons for remaining lives
    float ship_x = 24.0f;
    for (int i = 1; i < ship.lives; i++) {
        const char* path = IMG_SPACESHIP;
        Pico_Rel_Rect crop = { '!', {0, 0, 16, 8}, PICO_ANCHOR_NW, NULL };
        pico_layer_image_mode('=', "ship", path);
        pico_layer_sub_mode('=', "/crop/ship/life", "ship", &crop);
        Pico_Rel_Rect dst = { '!', {ship_x, 248.0f, 16, 8}, PICO_ANCHOR_SW, NULL };
        pico_output_draw_layer("/crop/ship/life", &dst);
        ship_x += 16.0f;
    }
}
