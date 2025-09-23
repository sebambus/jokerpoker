#include "hand.h"

#include <ncurses.h>
#include "color.h"

void hand::add(card c) {
    cards.push_back(c);
    selected.push_back(false);
}

card hand::pop() {
    selected.pop_back();
    card c = cards.back();
    cards.pop_back();
    return c;
}

std::vector<card> hand::popSelected() {
    std::vector<card> out;
    for(int i = 0; i < cards.size(); i++)
        if(selected[i]) {
            out.push_back(cards[i]);
            cards.erase(cards.begin()+i);
            selected.erase(selected.begin()+i);
        }
    return out;
}

void hand::print() {
    int x, y;
    getyx(stdscr, y, x);
    for(int i = 0; i < cards.size(); i++) {
        if(selected[i])
            move(y+1, getcurx(stdscr));
        else
            move(y, getcurx(stdscr));
        cards[i].print();
        setcolor(COLOR_WHITE, COLOR_BLACK);
        printw(" ");
    }
}