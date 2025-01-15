#pragma once

struct Shield {
    int x;
    bool bits[352];

    void set_bit(int x, int y, bool value);
    bool get_bit(int x, int y);
    void deploy(int x);

    void draw();
    // bool collidedWithShot(Shot *shot);
    // void collideWithHorde(const Horde &horde);
};