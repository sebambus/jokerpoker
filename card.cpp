#include "card.h"

#include "color.h"
#include "debug.h"

card::card(){

}

// new card of value "v" and suit "s"
card::card(int value, suit suit){
    cardValue = value;
    cardSuit = suit;
    cardEnhancement = BASE_CARD;
    cardSeal = NO_SEAL;
}

card::card(item i) {
    cardValue = i.val % 13;
    cardSuit = (suit) (i.val / 13);
}

std::string card::name(){
    return valueToString(cardValue) + " of " + suitToString(cardSuit) + "s";
}


// prints card in color with one rank char and one suit wchar_t
void card::print(WINDOW* win) {
    wchar_t suitChar; // unicode string

    int x, y;
    getyx(win, y, x);

    switch (cardSuit) {
    case SPADE:
        suitChar = L'\u2660';
        break;
    case HEART:
        suitChar = L'\u2665';
        break;
    case CLUB:
        suitChar = L'\u2663';
        break;
    case DIAMOND:
        suitChar = L'\u2666';
        break;
    default:
        suitChar = L'?';
    }

    for(int i = 0; i < 3; i++) {
        wmove(win, y+i, x);
        for(int j = 0; j < 4; j++) {
            short fg = suitToColor(cardSuit);
            short bg = COLOR_WHITE;
            wchar_t c = ' ';

            switch (cardEnhancement) {
            case GOLD_CARD:
                bg = COLOR_YELLOW;
                break;
            case BONUS_CARD:
                if(j%3 == 0)
                    bg = COLOR_BLUE;
                break;
            case MULT_CARD:
                if(i == 1)
                    bg = COLOR_RED;
                break;
            case WILD_CARD:
                suitChar = '*';
                break;
            case GLASS_CARD:
                bg = COLOR_BLACK;
                break;
            case STONE_CARD:
                fg = COLOR_WHITE;
            case STEEL_CARD:
                bg = COLOR_BLUE;
                break;
            case LUCKY_CARD:
                if((i+j)%2 == 0)
                    bg = COLOR_YELLOW;
                break;
            }

            if(i*3-j*2 == 0 && cardEnhancement != STONE_CARD)
                c = valueToChar(cardValue);
            if(2*j-i == 2 && cardEnhancement != STONE_CARD)
                c = suitChar;
            if(j-i == 3) {
                if(cardSeal == GOLD_SEAL) fg = COLOR_YELLOW;
                if(cardSeal == RED_SEAL) fg = COLOR_RED;
                if(cardSeal == BLUE_SEAL) fg = COLOR_BLUE;
                if(cardSeal == PURPLE_SEAL) fg = COLOR_MAGENTA;
                if(cardSeal != NO_SEAL) c = L'\u25cf';
            }

            cchar_t cch;
            setcchar(&cch, &c, 0, fg+16*bg, NULL);
            wadd_wch(win, &cch);
        }
    }

    wmove(win, y, x+4);
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

std::string suitToString(suit s){
    switch(s){
        case DIAMOND:
            return "Diamond";
        case SPADE:
            return "Spade";
        case HEART:
            return "Heart";
        case CLUB:
            return "Club";
    }
}

std::string valueToString(int v){
    if (v > 1 && v < 11){
        return std::to_string(v);
    }
    switch(v){
        case 1:
            return "Ace";
        case 11:
            return "Jack";
        case 12:
            return "Queen";
        case 13:
            return "King";
    }
    return "X";
}