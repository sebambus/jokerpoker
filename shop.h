#pragma once
#include <vector>
#include "item.h"

class game;

class shop {
public:
    game *g;
    voucher v;
    std::vector<item> items;

    shop(game *g);
    void run();
    item generateItem();
    voucher generateVoucher();
    void generatePurchaseables();
    void refresh();
};