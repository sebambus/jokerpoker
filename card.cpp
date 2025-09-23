#include "card.h"

#include <ncurses.h>
#include "color.h"

card::card(int value, suit suit){
    cardValue = value;
    cardSuit = suit;
}

void card::print() {
    char suitChar, valChar;
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

    switch (cardValue) {
    case 13:
        suitChar = 'K';
        break;
    case 12:
        suitChar = 'Q';
        break;
    case 11:
        suitChar = 'J';
        break;
    case 10:
        suitChar = 'T';
        break;
    case 1:
        suitChar = 'A';
        break;
    }

    printw("%c%c", valChar, suitChar);
}