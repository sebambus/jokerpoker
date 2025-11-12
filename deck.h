#pragma once
#include <vector>

#include "card.h"
class deck {
public:
    std::vector<card> cards;

    // adds the standard 52 poker cards to the deck
    void fillDeck();
    // shuffles cards using fisher-yates algorithm
    void shuffle();
    int suitCount(suit s);
    int cardCount(int v);
    int specificCount(suit s, int v);
    void removeCard(card c);
};