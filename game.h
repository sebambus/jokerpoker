#pragma once
#include <bitset>

#include "deck.h"
#include "window.h"
#include "item.h"
#include "shop.h"
#include "level.h"

enum handTableIndices : int{
    MULT,
    FLAT,
    LEVEL,
    TIMES_PLAYED
};

enum selectableScreen : int{
    CONSUMABLE_SCREEN,
    JOKER_SCREEN
};

class game {
public:
    int money;
    int ante;
    int round;
    std::vector<item> consumables;
    std::vector<item> jokers;
    int handTable[13][4]; //13 hands, 4 fields, example: handtable[HIGH][MULT]
    std::bitset<VOUCHER_COUNT> vouchers;
    deck d;
    window levelInfo;
    window gameInfo;
    window mainScreen;
    window specialScreen;
    window jokerScreen;
    window cardInfo;
    window peekScreen;
    shop* s;
    level* l;
    
    selectableScreen focusScreen = CONSUMABLE_SCREEN;
    int currConsumable = 0;
    int currJoker = 0;

    game();
    void run();
    void runinit();
    void runswitch();
    void runupdate();
    int getPlays();
    int getDiscards();
    void initHandTable();
    void gain(item);
    bool spend(int);
    void swapFocus();
    void changeConsumable(int by);
    void changeJoker(int by);
};