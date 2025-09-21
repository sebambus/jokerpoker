#include <cstdlib>
#include "deck.h"

void deck::shuffle() {
    for(int i = 0; i < cards.size(); i++) {
        int r = i + rand() % (cards.size() - i);
        card temp = cards[r];
        cards[r] = cards[i];
        cards[i] = temp;
    }
}