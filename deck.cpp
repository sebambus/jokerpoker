#include "deck.h"

#include <random>
#include <time.h>
#include <cstdlib>
#include "card.h"

// adds the standard 52 poker cards to the deck
void deck::fillDeck() {
    for (int i = 1; i < 14; i++)
        for(int j = 0; j < 4; j++)  // using the integer version of the enum to make for loop easier
            cards.push_back(card(i, (suit) j));
}

// shuffles cards using fisher-yates algorithm
void deck::shuffle() {
    srand(time(0)); //seed rand num generator
    for(int i = 0; i < cards.size(); i++) {
        int r = i + rand() % (cards.size() - i);
        card temp = cards[r];
        cards[r] = cards[i];
        cards[i] = temp;
    }
}

// count all cards of suit s
int deck::suitCount(suit s){ 
    int sum = 0;
    for (card c : cards){
        if (c.cardSuit == s)
            sum ++;
    }
    return sum;
}

// count all cards of value v
int deck::cardValueCount(int v){ 
    int sum = 0;
    for (card c : cards){
        if (c.cardValue == v)
            sum ++;
    }
    return sum;
}

int deck::specificCount(suit s, int v){ // count all cards of suit s and value v
    int sum = 0;
    for (card c : cards){
        if (c.cardSuit == s && c.cardValue == v)
            sum ++;
    }
    return sum;
}

void deck::removeCard(card c){
    for (int i = 0; i < cards.size(); i++){
        card x = cards[i];
        if (c.cardValue == x.cardValue &&
        c.cardSuit == x.cardSuit &&
        c.cardEnhancement == x.cardEnhancement &&
        c.cardSeal == x.cardSeal)
            cards.erase(cards.begin()+i);
    }
}