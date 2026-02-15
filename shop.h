#pragma once
#include <vector>
#include "item.h"
#include <string>
#include "hand.h"

class game;

typedef struct pack {
    itemtype type;
    int size;
    int cost;
} pack;
std::string name(pack);
std::string description(pack p);

enum shopMode : int {
    DEFAULT_MODE,
    PACK_MODE,
    SHOP_MODE_SIZE
};

class shop {
public:
    game *g;
    voucher v;
    std::vector<item> items;
    std::vector<pack> packs;
    std::vector<item> packItems;  

    hand modifyableCards;

    int rerollCount;
    shopMode mode = DEFAULT_MODE;
    int packUsesLeft = 0;

    shop(game *g);
    void run();
    item generateItem(itemtype);
    item generateItem();
    voucher generateVoucher();
    pack generatePack();
    void open(pack);
    void closePack();
    void reroll();
    void reopen();
    void fillShopItems();
    int shopSize();
};