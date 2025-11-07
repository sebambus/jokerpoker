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
    const wchar_t* suitChar; // unicode string

    int x, y;
    getyx(win, y, x);

    switch (cardSuit) {
    case SPADE:
        suitChar = L"\u2660";
        break;
    case HEART:
        suitChar = L"\u2665";
        break;
    case CLUB:
        suitChar = L"\u2663";
        break;
    case DIAMOND:
        suitChar = L"\u2666";
        break;
    default:
        suitChar = L"?";
    }

    setcolor(win, suitToColor(cardSuit), COLOR_WHITE);

    wprintw(win, "%c", valueToChar(cardValue));
    waddwstr(win, suitChar);
    wprintw(win, "  ");
    wmove(win, y+1, x);
    wprintw(win, "    ");
    wmove(win, y+2, x);
    wprintw(win, "  ");
    waddwstr(win, suitChar);
    wprintw(win, "%c", valueToChar(cardValue));

    setcolor(win, suitToColor(cardSuit), COLOR_WHITE);
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

int suitToColor(suit s){
    switch (s){
        case SPADE:
            return COLOR_BLACK;
        case HEART:
            return COLOR_RED;
        case CLUB:
            return COLOR_BLUE;
        case DIAMOND:
            return COLOR_YELLOW;
    }
    return COLOR_BLACK;
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