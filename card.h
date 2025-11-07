#pragma once
#include <ncursesw/ncurses.h>

#include "item.h"

enum suit {
    SPADE,
    CLUB,
    HEART,
    DIAMOND
};

enum enhancement {
    BASE_CARD,
    BONUS_CARD,
    MULT_CARD,
    WILD_CARD,
    GLASS_CARD,
    STEEL_CARD,
    STONE_CARD,
    GOLD_CARD,
    LUCKY_CARD
};

enum seal {
    NO_SEAL,
    GOLD_SEAL,
    RED_SEAL,
    BLUE_SEAL,
    PURPLE_SEAL
};

class card{
public:
    int cardValue = 0;
    suit cardSuit = SPADE;
    enhancement cardEnhancement = WILD_CARD;
    seal cardSeal = NO_SEAL;

    // new card of value "v" and suit "s"
    card(int v, suit s);
    card(item);
    // prints card in color with one rank char and one suit wchar_t
    void print(WINDOW*);


};
    char suitToChar(suit s);
    int suitToColor(suit s);
    char valueToChar(int v);