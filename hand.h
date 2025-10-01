#pragma once
#include <vector>
#include "deck.h"
#include <map>

enum handtype {
    FIVE_FLUSH,
    HOUSE_FLUSH,
    FIVE_KIND,
    ROYAL_FLUSH,
    STRAIGHT_FLUSH,
    FOUR_KIND,
    FULL_HOUSE,
    FLUSH,
    STRAIGHT,
    THREE_KIND,
    TWO_PAIR,
    PAIR,
    HIGH
};


const char* handName(handtype h);

class hand : public deck {
public:
    std::vector<bool> selected;
    int cursor;

    // creates blank hand
    hand();
    // creates hand containing given cards
    hand(std::vector<card>);
    void add(card);
    // deletes and returns last card in hand
    card pop();
    // deletes and returns a vector of all selected cards
    std::vector<card> popSelected();
    // prints all cards in hand, with cursor highlighted and selected cards shifted
    void print();
    // moves cursor right by moveBy
    void moveCursor(int moveBy);
    void sortBySuit();
    void sortByValue();
    // selects/deselects the card under the cursor
    void selectCursor();
    int cardsSelected();
    handtype scoreType();
private:
    std::vector<card> subsortBySuit(std::vector<card> sub);
    std::vector<card> subsortByValue(std::vector<card> sub);
};