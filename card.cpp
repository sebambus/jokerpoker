#include "card.h"

#include "color.h"

// new card of value "v" and suit "s"
card::card(int value, suit suit){
    cardValue = value;
    cardSuit = suit;
}

card::card(item i) {
    cardValue = i.val % 13;
    cardSuit = (suit) (i.val / 13);
}

// prints card in color with one rank char and one suit wchar_t
void card::print(WINDOW* win) {
    char valChar;
    const wchar_t* suitChar; // unicode string

    switch (cardSuit) {
    case SPADE:
        suitChar = L"\u2660";
        setcolor(win, COLOR_BLACK, COLOR_WHITE);
        break;
    case HEART:
        suitChar = L"\u2665";
        setcolor(win, COLOR_RED, COLOR_WHITE);
        break;
    case CLUB:
        suitChar = L"\u2663";
        setcolor(win, COLOR_BLUE, COLOR_WHITE);
        break;
    case DIAMOND:
        suitChar = L"\u2666";
        setcolor(win, COLOR_YELLOW, COLOR_WHITE);
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

    wprintw(win, "%c", valChar);
    // prints unicode string
    waddwstr(win, suitChar);
}

char suitToChar(suit s){
    switch(s){
        case SPADE:
            return 'S';
        case DIAMOND:
            return 'D';
        case HEART:
            return 'H';
        case CLUB:
            return 'C';
    }
    return 'X';
}

char valueToChar(int v){
    if (v > 1 && v < 10){
        return '0' + v;
    }
    switch(v){
        case 1:
            return 'A';
        case 10:
            return 'T';
        case 11:
            return 'J';
        case 12:
            return 'Q';
        case 13:
            return 'K';
    }
    return 'X';
}