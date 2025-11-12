#pragma once
#include <bitset>

#include "deck.h"
#include "window.h"
#include "item.h"

enum handTableIndices : int{
    MULT,
    FLAT,
    LEVEL,
    TIMES_PLAYED
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
};