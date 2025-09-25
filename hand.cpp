#include "hand.h"

#include <ncurses.h>
#include "color.h"

// creates blank hand
hand::hand() {
    cursor = 0;
}

// creates hand containing given cards
hand::hand(std::vector<card> list) {
    cards = list;
    for(int i = 0; i < cards.size(); i++)   // fills selected with false
        selected.push_back(false);
    cursor = 0;
}

void hand::add(card c) {
    cards.push_back(c);
    selected.push_back(false);
}

// deletes and returns last card in hand
card hand::pop() {
    selected.pop_back();
    card c = cards.back();
    cards.pop_back();
    if(cursor == cards.size()) cursor--;    // ensures cursor remains in hand
    return c;
}

// deletes and returns a vector of all selected cards
std::vector<card> hand::popSelected() {
    std::vector<card> out;

    for(int i = 0; i < cards.size(); i++)
        if(selected[i]) {                   // loops over selected cards
            out.push_back(cards[i]);
            cards.erase(cards.begin()+i);
            selected.erase(selected.begin()+i);
             
            if(cursor >= i) cursor--;   // shifts cursor if needed to stay on same card
            i--;    // repeats loop on current line, since there is now a new card there
        }

    return out;
}

// prints all cards in hand, with cursor highlighted and selected cards shifted
void hand::print() {
    int x, y;
    getyx(stdscr, y, x);
    for(int i = 0; i < cards.size(); i++) {
        //highlight cursor
        if(i == cursor) attron(A_REVERSE);
        // shift card up or down based on selection
        if(selected[i])
            move(y+1, getcurx(stdscr));
        else
            move(y, getcurx(stdscr));

        cards[i].print();
        // reset color attributes
        if(i == cursor) attroff(A_REVERSE);
        setcolor(COLOR_WHITE, COLOR_BLACK);
        // spacing
        printw(" ");
    }
    // movve cursor to beneath
    move(y+2, x);
}

// moves cursor right by moveBy
void hand::moveCursor(int moveBy){
    int newPosition = cursor + moveBy;
    if (newPosition > selected.size() -1 || newPosition < 0)
        return; //dont move cursor if attempting to move beyond hand
    cursor += moveBy;
}