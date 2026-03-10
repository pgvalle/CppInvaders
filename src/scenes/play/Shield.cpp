#include "Shield.hpp"
#include "CppInvaders.hpp"
#include <vector>
#include <cstring>
#include <cmath>

#define ROUND(x) ((int)std::round(x))

static SDL_Point damage_pattern_1[] = {
                   {-1,0},
    {-3,1},                       {1,1},
                   {-1,2}, {0,2},        {2,2},
           {-2,3}, {-1,3}, {0,3}, {1,3},
    {-3,4},        {-1,4}, {0,4}, {1,4},
           {-2,5}, {-1,5}, {0,5}, {1,5}, {2,5},
    {-3,6},        {-1,6}, {0,6}, {1,6},
           {-2,7},         {0,7},        {2,7},
};

static SDL_Point damage_pattern_2[] = {
    {-3,0},                        {1,0},               {4,0},
                    {-1,1},                      {3,1},
            {-2,2}, {-1,2}, {0,2}, {1,2}, {2,2}, {3,2},
    {-3,3}, {-2,3}, {-1,3}, {0,3}, {1,3}, {2,3}, {3,3}, {4,3},
    {-3,4}, {-2,4}, {-1,4}, {0,4}, {1,4}, {2,4}, {3,4}, {4,4},
            {-2,5}, {-1,5}, {0,5}, {1,5}, {2,5}, {3,5},
                    {-1,6},               {2,6},
    {-3,7},                 {0,7},                      {4,7},
};

void Shield::deploy(int x_) {
    x = x_;
    memset(bits, true, 352);

    for (int i = 0; i < 4; i++) { // line
        for (int j = 0; j < 4 - i; j++) { // top edges by column
            bits[22 * i + j] = bits[22 * i - j + 21] = false;
        }
        
        const int l = 15 - i; // bottom-up
        for (int j = 5 + i; j < 18 - i; j++) { // // bottom part by column
            bits[22 * l + j] = bits[22 * l - j + 21] = false;
        }
    }
}

bool Shield::damage(Bullet *shot) {
    int shotx = ROUND(shot->x), shoty = ROUND(shot->y);
    int point_count = 0;

    for (int y = 2; y < 5; y++) {
        int relx = shotx - x, rely = shoty - 192 + y;
        bool in_bounds = (0 <= relx && relx < 22 && 0 <= rely && rely < 16);
        if (in_bounds) {
            point_count += (bits[22 * rely + relx] ? 1 : 0);
        }
    }

    if (point_count < 1) {
        return false;
    }

    if (shot->vy >= 0) {
        for (SDL_Point off : damage_pattern_1) {
            int relx = shotx - x + off.x, rely = shoty - 192 + off.y;
            bool in_bounds = (0 <= relx && relx < 22 && 0 <= rely && rely < 16);
            if (in_bounds) {
                bits[22 * rely + relx] = false;
            }
        }
    }
    else {
        for (SDL_Point off : damage_pattern_2) {
            int relx = shotx - x + off.x, rely = shoty - 192 + off.y;
            bool in_bounds = (0 <= relx && relx < 22 && 0 <= rely && rely < 16);
            if (in_bounds) {
                bits[22 * rely + relx] = false;
            }
        }
    }

    return true;
}

void Shield::damage(SDL_Rect rect) {
    SDL_Rect shield_rect = { x, 192, 22, 16 }, result;
    if (!SDL_IntersectRect(&rect, &shield_rect, &result)) {
        return;
    }

    for (int yi = 0; yi < result.h; yi++) {
        for (int xi = 0; xi < result.w; xi++) {
            bits[22 * (yi + result.y - 192) + xi + result.x - x] = false;
        }
    }
}

void Shield::draw() const {
    std::vector<Pico_Rel_Pos> points;
    for (int i = 0; i < 352; i++) {
        if (bits[i]) {
            points.push_back({ '!', {(float)(x + i % 22), (float)(192 + i / 22)}, PICO_ANCHOR_NW, NULL });
        }
    }

    pico_set_color_draw(PICO_COLOR_GREEN);
    
    // Draw in batches to avoid stack overflow in pico's pico_output_draw_pixels
    const int batch_size = 64;
    for (size_t i = 0; i < points.size(); i += batch_size) {
        int n = (points.size() - i < batch_size) ? points.size() - i : batch_size;
        pico_output_draw_pixels(n, &points[i]);
    }
}
