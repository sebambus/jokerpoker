#include "card.h"

#include <ncursesw/ncurses.h>
#include "color.h"

card::card(int value, suit suit){
    cardValue = value;
    cardSuit = suit;
}

void card::print() {
    char valChar;
    const wchar_t* suitChar;
    switch (cardSuit) {
    case SPADE:
        suitChar = L"\u2660";
        setcolor(COLOR_BLACK, COLOR_WHITE);
        break;
    case HEART:
        suitChar = L"\u2665";
        setcolor(COLOR_RED, COLOR_WHITE);
        break;
    case CLUB:
        suitChar = L"\u2663";
        setcolor(COLOR_BLUE, COLOR_WHITE);
        break;
    case DIAMOND:
        suitChar = L"\u2666";
        setcolor(COLOR_YELLOW, COLOR_WHITE);
        break;
    default:
        suitChar = L"?";
    }

    switch (cardValue) {
    case 13:
        valChar = 'K';
        break;
    case 12:
        valChar = 'Q';
        break;
    case 11:
        valChar = 'J';
        break;
    case 10:
        valChar = 'T';
        break;
    case 1:
        valChar = 'A';
        break;
    default:
        valChar = '0' + cardValue;
    }

    printw("%c", valChar);
    mvaddwstr(getcury(stdscr), getcurx(stdscr), suitChar);
}