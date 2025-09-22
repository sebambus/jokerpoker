#include "deck.h"

#include <cstdlib>

deck::deck(){
    for (int i = 1; i < 14; i++)
        for(int j = 0; j < 4; j++)  // using the integer version of the enum to make for loop easier
            cards.push_back(card(i, (suit) j));
}

void deck::shuffle() {
    for(int i = 0; i < cards.size(); i++) {
        int r = i + rand() % (cards.size() - i);
        card temp = cards[r];
        cards[r] = cards[i];
        cards[i] = temp;
    }
}