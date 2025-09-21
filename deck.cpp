#include <cstdlib>
#include "deck.h"
#include "card.h"

deck::deck(){
    card newCard(1, SPADE);
}

void deck::shuffle() {
    for(int i = 0; i < cards.size(); i++) {
        int r = i + rand() % (cards.size() - i);
        card temp = cards[r];
        cards[r] = cards[i];
        cards[i] = temp;
    }
}