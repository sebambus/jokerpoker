#pragma once
#include <bitset>

#include "deck.h"
#include "window.h"
#include "item.h"
#include "shop.h"
#include "level.h"
#include "blind.h"

enum handTableIndices : int{
    MULT,
    FLAT,
    LEVEL,
    TIMES_PLAYED
};

enum decktype : int {
    RED_DECK,
    BLUE_DECK,
    YELLOW_DECK,    // not implemented
    GREEN_DECK,     // not implemented
    BLACK_DECK,     // not implemented
    MAGIC_DECK,     // not implemented
    NEBULA_DECK,    // not implemented
    GHOST_DECK,     // not implemented
    ABANDONED_DECK, // not implemented
    CHECKERED_DECK, // not implemented
    ZODIAC_DECK,    // not implemented
    PAINTED_DECK,   // not implemented
    ANAGLYPH_DECK,  // not implemented
    PLASMA_DECK,    // not implemented
    ERRATIC_DECK,   // not implemented
    DECK_TYPE_COUNT
};

enum screentype;

enum phase : int{
    DECK_PHASE,
    SHOP_PHASE,
    BLINDS_PHASE,
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
    std::vector<tag> tags;
    std::vector<bossBlindType> completedBossBlinds;
    blind currBlinds[3];
    int handTable[13][4]; //13 hands, 4 fields, example: handtable[HIGH][MULT]
    int antebases[9] = {100, 300, 800, 2000, 5000, 11000, 20000, 35000, 50000};
    std::bitset<VOUCHER_COUNT> vouchers;
    deck d;
    window levelInfo;
    window gameInfo;
    window mainScreen;
    window specialScreen;
    window jokerScreen;
    window cardInfo;
    window peekScreen;
    window playingCardInfo;
    shop* s;
    level* l;
    decktype currentDeck = DECK_TYPE_COUNT; // used to track which deck the player is currently using, if any. DECK_TYPE_COUNT means no deck is being used
    
    screentype focusScreen = CONSUMABLE_SCREEN;
    int currConsumable = 0;
    int currJoker = 0;
    int currShopItem = 0;

    bool running;

    game();
    void run();
    void runinit();
    void runswitch();
    void universalInput(char c);
    void levelInput(char c);
    void blindsInput(char c);
    void shopInput(char c);
    void runupdate();
    void useShopItem(int currItem);
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
    void makeBlinds();
    void skipBlind();
};