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

    std::vector<card>sortedSpades = subsortByValue(spades);
    std::vector<card>sortedHearts = subsortByValue(hearts);
    std::vector<card>sortedDiamonds = subsortByValue(diamonds);
    std::vector<card>sortedClubs = subsortByValue(clubs);

    for (int i = 0; i < sortedSpades.size(); i++){
        cards.push_back(sortedSpades.at(i));
    }
    for (int i = 0; i < sortedHearts.size(); i++){
        cards.push_back(sortedHearts.at(i));
    }
    for (int i = 0; i < sortedDiamonds.size(); i++){
        cards.push_back(sortedDiamonds.at(i));
    }
    for (int i = 0; i < sortedClubs.size(); i++){
        cards.push_back(sortedClubs.at(i));
    }

    return;
}



void hand::sortByValue(){
    std::vector<card> sortedVector;

    for (int i = 1; i < 15; i++) //for each possible value, check each card
    {
        for (int j = 0; j < cards.size(); j++)
        {
            if (cards.at(j).cardValue == i) //if card is that value, add it the sorted vector
                sortedVector.push_back(cards.at(j));
        }
    }
    
    cards.clear();

    for (int i = 0; i < sortedVector.size(); i++)
    {
        cards.push_back(sortedVector.at(i));
    }
    

    return;
}

//helper functions for sorting.
//take vectors split up with one method, then sort them using the other
std::vector<card> hand::subsortByValue(std::vector<card> sub){ //sorts each suit group by value
    std::vector<card> sortedVector;
    for (int i = 1; i < 15; i++) //for each possible value, check each card
    {
        for (int j = 0; j < sub.size(); j++)
        {
            if (sub.at(j).cardValue == i) //if card is that value, add it the sorted vector
                sortedVector.push_back(sub.at(j));
        }
    }

    return sortedVector;
}

std::vector<card> hand::subsortBySuit(std::vector<card> sub){
    return sub;
}

