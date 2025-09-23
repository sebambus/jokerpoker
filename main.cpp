#define _XOPEN_SOURCE_EXTENDED // needed to run unicode??
#include <ncursesw/ncurses.h>
#include <vector>
#include <locale>
#include "deck.h"
#include "card.h"
#include "color.h"
#include "hand.h"

deck d;

void playlevel() {
    d.fillDeck();
    d.shuffle();
    hand h;
    for (int i = 0; i < 8; i++) {
        h.add(d.cards.back());
        d.cards.pop_back();
    }
    h.print();
}

int main() {
    setlocale(LC_ALL, "");
    initscr();             // Start curses modeee
    start_color();
    initcolors();

    playlevel();
    refresh();             // Print it on to the real screen

    getchar();

    endwin();              // End curses mode
    return 0;
}