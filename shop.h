#pragma once
#include <vector>
#include "item.h"
#include <string>

class game;

typedef struct pack {
    itemtype type;
    int size;
} pack;
std::string name(pack);

// this is a hacky solution to the hop, so i could have a vector of everything in the shop
class shopItem{
public:
    int typeOfItem = 0; // 0 for item, 1 for pack, 2 for voucher
    item i;
    pack p;
    voucher v;
    int cost = 0;

    std::string getName();
    shopItem();
    shopItem(item n);
    shopItem(pack n);
    shopItem(voucher n);
};

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
    std::vector<shopItem> shopItems;
    std::vector<shopItem> packItems;
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
    void reroll();
    void reopen();
    void fillShopItems();
};