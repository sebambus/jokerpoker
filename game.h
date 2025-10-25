#pragma once
#include <bitset>

#include "deck.h"
#include "window.h"
#include "voucher.h"
#include "item.h"

class game {
public:
    int money;
    int ante;
    int round;
    std::vector<item> consumables;
    std::bitset<VOUCHER_COUNT> vouchers;
    deck d;
    window levelInfo;
    window gameInfo;
    window mainScreen;
    window specialScreen;
    window cardInfo;

    game();
    int getPlays();
    int getDiscards();
};