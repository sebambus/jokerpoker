#include <ncurses.h>
#include <vector>
#include "deck.h"
#include "card.h"

deck d;

void printcard(card c) {
    printw("%d ", c.cardValue);
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