#pragma once
#include <vector>
#include "item.h"
#include <string>
#include "hand.h"

class game;

typedef struct pack {
    itemtype type;
    int size;
} pack;
std::string name(pack);
std::string description(pack p);

enum shopItemType : int {
    SI_CARD,
    SI_VOUCHER,
    SI_PACK,
    SI_ITEM,
    SI_TYPE_SIZE
};

// this is a hacky solution to the hop, so i could have a vector of everything in the shop
class shopItem{
public:
    shopItemType typeOfItem = SI_CARD; // 0 for item, 1 for pack, 2 for voucher, 3 for card
    item i;
    pack p;
    voucher v;
    card c;
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
};