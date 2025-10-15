#pragma once

class game;
enum voucher : int;

class shop{
public:
    game *g;
    voucher v;

    shop(game *g);
    void run();
};