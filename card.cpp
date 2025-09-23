#include "card.h"

#include <ncurses.h>
#include "color.h"

card::card(int value, suit suit){
    cardValue = value;
    cardSuit = suit;
}

void card::print() {
    char suitChar;
    switch (cardSuit) {
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

    printw("%d%c ", cardValue, suitChar);
}