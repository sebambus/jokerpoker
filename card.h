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
    MULT_CARD
};

class card{
public:
    int cardValue = 0;
    suit cardSuit = SPADE;
    enhancement cardEnhancement = BASE_CARD;

    // new card of value "v" and suit "s"
    card(int v, suit s);
    card(item);
    // prints card in color with one rank char and one suit wchar_t
    void print(WINDOW*);


};
    char suitToChar(suit s);
    int suitToColor(suit s);
    char valueToChar(int v);