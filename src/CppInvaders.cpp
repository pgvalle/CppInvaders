#include "CppInvaders.h"
#include "screens/Screens.h"

void CppInvaders::load_scoreboard() {
    score = 0;
    hi_score = 0;

    FILE *file = fopen(SCOREBOARD_FILE, "r");
    if (!file) {
        file = fopen(SCOREBOARD_FILE, "w");
        fclose(file);
        return;
    }

    if (!file) {
        printf("Couldn't open scoreboard file\n");
        return;
    }

    fscanf(file, "%6d", &hi_score);
    fclose(file);
}

void CppInvaders::save_scoreboard() {
    // open file to write (will exist bc load was called b4)
    // write hi_score to file
    // closefile
    FILE *file = fopen(SCOREBOARD_FILE, "w");
    if (!file) {
        printf("Couldn't open scoreboard file\n");
        return;
    }

    fprintf(file, "%6d", hi_score);
    fclose(file);
}

void CppInvaders::add_to_score(int value) {
    score += value;
    if (hi_score < score) {
        hi_score = score;
        save_scoreboard();
    }
}

void CppInvaders::draw_scoreboard() {
    static char text[32];
    sprintf(text, "%06d          %06d", score, hi_score);
    
    pico_set_color_draw(WHITE);
    pico_output_draw_text({ 8, 8 }, "YOUR SCORE      HIGH-SCORE");
    pico_output_draw_text({ 24, 24 }, text);
}

void CppInvaders::draw_credit_counter() {
    static char text[16];
    sprintf(text, "CREDIT %02d", credits);

    pico_set_color_draw(WHITE);
    pico_output_draw_text({ 144, 240 }, text);
}

CppInvaders::CppInvaders() {
    win = SDL_CreateWindow(
        "CppInvaders - sdl",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        256,
        224,
        0
    );

    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_RenderSetScale(ren, 2, 2);

    fnt = TTF_OpenFont(FONT, 8);

    SDL_Texture *tex;
    SDL_Surface *sfc;

    sfc = IMG_LoadPNG(IMG_SPACESHIP);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_SPACESHIP, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_UFO);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_UFO, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_INV1);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_INV1, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_INV2);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_INV2, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_INV3);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_INV3, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_EXP1);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_EXP1, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_EXP2);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_EXP2, tex});
    SDL_FreeSurface(sfc);
    sfc = IMG_LoadPNG(IMG_EXP3);
    tex = SDL_CreateTextureFromSurface(ren, );
    assets.insert({IMG_EXP3, tex});
    SDL_FreeSurface(sfc);

    assets.insert({SFX_SPACESHIP_SHOOT, Mix_LoadWAV(SFX_SPACESHIP_SHOOT)});
    assets.insert({SFX_SPACESHIP_KILLED, Mix_LoadWAV(SFX_SPACESHIP_KILLED)});
    assets.insert({SFX_INVADER_KILLED, Mix_LoadWAV(SFX_INVADER_KILLED)});

    splash = new Splash;
    game = nullptr;
    pause = nullptr;
    over = nullptr;

    screen = SPLASH;
    credits = 99;
    load_scoreboard();

    should_quit = false;
}

CppInvaders::~CppInvaders() {
    save_scoreboard();

    delete splash;
    delete game;
    delete pause;
    delete over;

    TTF_CloseFont(fnt);
    SDL_DestroyRenderer(REN);
    SDL_DestroyWindow(WIN);
}

void CppInvaders::update_and_draw(float delta) {
    switch (screen) {
    case SPLASH:
        pico_assert(splash);
        splash->draw();
        splash->update(delta);
        break;
    case GAME:
        pico_assert(game);
        game->draw();
        game->update(delta);
        break;
    case PAUSE:
        pico_assert(pause);
        pause->draw();
        pause->update(delta);
        break;
    case OVER:
        pico_assert(over);
        over->draw();
        over->update(delta);
        break;
    }

    SDL_RenderPresent(REN);
}

void CppInvaders::process_event(const SDL_Event& event) {
    if (event.type == SDL_QUIT) {
        should_quit = true;
        return;
    }

    switch (screen) {
    case SPLASH:
        pico_assert(splash);
        splash->process_event(event);
        break;
    case GAME:
        pico_assert(game);
        game->process_event(event);
        break;
    case PAUSE:
        pico_assert(pause);
        pause->process_event(event);
        break;
    case OVER:
        pico_assert(over);
        over->process_event(event);
        break;
    }
}

void CppInvaders::draw_text(int x, int y, const char *text) {
    uint8_t r, g, b, a;
    SDL_GetRenderDrawColor(REN, &r, &g, &b, &a);

    SDL_Surface* sfc = TTF_RenderText_Blended(fnt, text, (SDL_Color){r,g,b,a});
    SDL_Texture* tex = SDL_CreateTextureFromSurface(ren, sfc);
    SDL_Rect rct = {x, y, sfc-w, sfc->h};

    SDL_RenderCopy(ren, tex, NULL, &rct);
    SDL_DestroyTexture(tex);
    SDL_FreeSurface(sfc);
}

void CppInvaders::draw_image(int x, int y, const char *image, const SDL_Rect &crop) {
    SDL_Texture *tex = (SDL_Texture *)assets.at(image);
    SDL_Rect dst = {x, y, crop.w, crop.h};
    SDL_RenderCopy(ren, tex, &crop, dst);
}

void CppInvaders::play_sound(const char *sound) {
    Mix_Music *mus = (Mix_Music *)assets.at(sound);
    Mix_PlayMusic(mus, 0);
}
