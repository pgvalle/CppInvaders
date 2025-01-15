#include "Shield.h"
#include "screens/Game.h"

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

void Shield::deploy(int x_) {
    memset(bits, true, 352);
    x = x_;

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

bool Shield::damage(Shot *shot) {
    // TODO: implement shield damage
    return false;
}

// bool Shield::collidedWithShot(Shot *shot)
// {
//     SDL_Rect shotRect = shot->getRect();

//     if (!SDL_HasIntersection(&shotRect, &outerBounds))
//         return false;

//     shot->y += rand() % 4;
//     int x = round(shot->x - 2), y = round(shot->y);
//     SDL_Point plist[] = {
//         {x + 2, y},

//         {x, y + 1},
//         {x + 4, y + 1},

//         {x + 2, y + 2},
//         {x + 3, y + 2},
//         {x + 5, y + 2},

//         {x + 1, y + 3},
//         {x + 2, y + 3},
//         {x + 3, y + 3},
//         {x + 4, y + 3},

//         {x, y + 4},
//         {x + 2, y + 4},
//         {x + 3, y + 4},
//         {x + 4, y + 4},

//         {x + 1, y + 5},
//         {x + 2, y + 5},
//         {x + 3, y + 5},
//         {x + 4, y + 5},
//         {x + 5, y + 5},

//         {x, y + 6},
//         {x + 2, y + 6},
//         {x + 3, y + 6},
//         {x + 4, y + 6},

//         {x + 1, y + 7},
//         {x + 3, y + 7},
//         {x + 5, y + 7},
//     };

//     bool decay = false;
//     for (SDL_Point p : plist)
//     {
//         if (getPoint(p.x, p.y))
//         {
//             decay = true;
//             break;
//         }
//     }

//     if (decay)
//     {
//         for (SDL_Point p : plist)
//             setPoint(p.x, p.y, false);
//     }

//     return decay;
// }

void Shield::draw() {
    std::vector<SDL_Point> points;
    for (int i = 0; i < 352; i++) {
        if (bits[i]) {
            points.push_back({ x + i % 22, 192 + i / 22 });
        }
    }

    pico_set_color_draw(GREEN);
    pico_output_draw_pixels(points.data(), 352);
}

// void Shield::collideWithHorde(const Horde &horde)
// {
//     for (int i = 0; i < 11; i++)
//     {
//         SDL_Rect rect = horde.invaders[i].getRect(), r2;
//         if (SDL_IntersectRect(&outerBounds, &rect, &r2))
//         {
//             for (int y = 0; y < r2.h; y++)
//             {
//                 for (int x = 0; x < r2.w; x++)
//                     setPoint(r2.x + x, r2.y + y, false);
//             }
//         }
//     }
// }