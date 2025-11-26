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

enum screentype;

enum phase : int{
    SHOP_PHASE,
    LEVEL_PHASE,
    PHASE_COUNT
};

class game {
public:
    int money = 0;
    int ante = 1;
    int round = 1;
    int phase; 
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
    
    screentype focusScreen = CONSUMABLE_SCREEN;
    int currConsumable = 0;
    int currJoker = 0;
    int currShopItem = 0;

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
    void changeFocus(screentype scr);
    void moveMenuCursor(int by);
    void updateMenuScreens();
    void changeConsumable(int by);
    void changeJoker(int by);
    void changeShopItem(int by);
};