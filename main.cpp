#include <ncurses.h>
#include <vector>
#include "deck.h"
#include "card.h"
#include "color.h"

deck d;

void printcard(card c) {
    char suitChar;
    if (c.cardSuit == SPADE)
        suitChar = 'S';
    else if (c.cardSuit == HEART)
        suitChar = 'H';
    else if (c.cardSuit == CLUB)
        suitChar = 'C';
    else if (c.cardSuit == DIAMOND)
        suitChar = 'D';

    printw("%d%c ", c.cardValue, suitChar);
}

void playlevel() {
    d.shuffle();
    std::vector<card> hand;
    for (int i = 0; i < 8; i++) {
        hand.push_back(d.cards.back());
        d.cards.pop_back();
        printcard(hand[i]);
    }
}

int main() {
    initscr();             // Start curses modeee

    playlevel();
    refresh();             // Print it on to the real screen

    getchar();

    endwin();              // End curses mode
    return 0;
}