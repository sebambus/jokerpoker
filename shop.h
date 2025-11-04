#pragma once
#include <vector>
#include "item.h"

class game;

typedef struct pack {
    itemtype type;
    int size;
} pack;

class shop {
public:
    game *g;
    voucher v;
    std::vector<item> items;
    std::vector<pack> packs;

    shop(game *g);
    void run();
    item generateItem(itemtype);
    item generateItem();
    voucher generateVoucher();
    pack generatePack();
    bool open(pack);
    void reroll();
    void reopen();
};