#include "hand.h"

#include <ncurses.h>
#include "color.h"

hand::hand() {
    cursor = 0;
}

hand::hand(std::vector<card> list) {
    cards = list;
    for(int i = 0; i < cards.size(); i++)
        selected.push_back(false);
    cursor = 0;
}

void hand::add(card c) {
    cards.push_back(c);
    selected.push_back(false);
}

card hand::pop() {
    selected.pop_back();
    card c = cards.back();
    cards.pop_back();
    cursor--;
    return c;
}

std::vector<card> hand::popSelected() {
    std::vector<card> out;
    for(int i = 0; i < cards.size(); i++)
        if(selected[i]) {
            out.push_back(cards[i]);
            cards.erase(cards.begin()+i);
            selected.erase(selected.begin()+i);
            if(cursor >= i) cursor--;
            i--;
        }
    return out;
}

void hand::print() {
    sortBySuit();
    int x, y;
    getyx(stdscr, y, x);
    for(int i = 0; i < cards.size(); i++) {
        if(i == cursor) attron(A_REVERSE);
        if(selected[i])
            move(y+1, getcurx(stdscr));
        else
            move(y, getcurx(stdscr));
        cards[i].print();
        setcolor(COLOR_WHITE, COLOR_BLACK);
        if(i == cursor) attroff(A_REVERSE);
        printw(" ");
    }
    move(y+2, x);
}

void hand::moveCursor(int moveBy){
    int newPosition = cursor + moveBy;
    if (newPosition > cards.size() -1 || newPosition < 0)
        return; //dont move cursor if attempting to move beyond hand
    cursor += moveBy;
}

void hand::sortBySuit(){
    std::vector<card> spades;
    std::vector<card> hearts;
    std::vector<card> diamonds;
    std::vector<card> clubs;

    for (int i = 0; i < cards.size(); i++)
    {
        suit cardSuit = cards.at(i).cardSuit;
        switch (cardSuit){
        case SPADE:
            spades.push_back(cards.at(i));
            break;
        case HEART:
            hearts.push_back(cards.at(i));
            break;
        case DIAMOND:
            diamonds.push_back(cards.at(i));
            break;
        case CLUB:
            clubs.push_back(cards.at(i));
            break;
        }        
    }
    
    cards.clear();
    
    for (int i = 0; i < spades.size(); i++){
        cards.push_back(spades.at(i));
    }
    for (int i = 0; i < hearts.size(); i++){
        cards.push_back(hearts.at(i));
    }
    for (int i = 0; i < diamonds.size(); i++){
        cards.push_back(diamonds.at(i));
    }
    for (int i = 0; i < clubs.size(); i++){
        cards.push_back(clubs.at(i));
    }

    return;
}

void hand::sortByValue(){
    return;
}