#include <ncurses.h>
#include <vector>
#include "deck.h"
#include "card.h"
#include "color.h"

deck d;

void printcard(card c) {
    char suitChar;
    switch (c.cardSuit) {
    case SPADE:
        suitChar = 'S';
        setcolor(COLOR_BLACK, COLOR_WHITE);
        break;
    case HEART:
        suitChar = 'H';
        setcolor(COLOR_RED, COLOR_WHITE);
        break;
    case CLUB:
        suitChar = 'C';
        setcolor(COLOR_BLUE, COLOR_WHITE);
        break;
    case DIAMOND:
        suitChar = 'D';
        setcolor(COLOR_YELLOW, COLOR_WHITE);
        break;
    default:
        suitChar = '?';
    }

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
    start_color();
    initcolors();

    playlevel();
    refresh();             // Print it on to the real screen

    getchar();

    endwin();              // End curses mode
    return 0;
}