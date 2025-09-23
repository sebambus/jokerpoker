#include <ncurses.h>
#include <vector>
#include "deck.h"
#include "card.h"
#include "color.h"

deck d;

void playlevel() {
    d.shuffle();
    std::vector<card> hand;
    for (int i = 0; i < 8; i++) {
        hand.push_back(d.cards.back());
        d.cards.pop_back();
        d.cards[i].print();
        setcolor(COLOR_WHITE, COLOR_BLACK);
        printw(" ");
    }
}

int main() {
    initscr();             // Start curses modeee
    start_color();
    initcolors();

    playlevel();
    refresh();             // Print it on to the real screen

    getchar();

    endwin();              // End curses mode
    return 0;
}