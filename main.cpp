#define _XOPEN_SOURCE_EXTENDED // needed to run unicode??
#include <ncursesw/ncurses.h>
#include <vector>
#include <locale>
#include "deck.h"
#include "card.h"
#include "color.h"
#include "hand.h"

deck d;
int score = 0;

void playlevel() {
    d.fillDeck();
    d.shuffle();

    hand h;
    // deals 8 cards
    for (int i = 0; i < 8; i++) {
        h.add(d.cards.back());
        d.cards.pop_back();
    }

    hand played;    
    while(true) {
        // update screen
        clear();
        h.print();
        played.print();
        refresh();

        // read keyboard input with switch
        switch (getchar()) {
            // q - quit
            case 'q':
                return;
            // h - left (vim)
            case 'h':
                h.moveCursor(-1);
                break;
            // l - right
            case 'l':
                h.moveCursor(1);
                break;
            // _ - select
            case ' ':
                h.selected[h.cursor] = !h.selected[h.cursor];
                break;
            // p - play
            case 'p':
                played = hand(h.popSelected());
                played.cursor = -1;
                break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "");  // switch to UTF-8??
    initscr();              // boot ncurses
    start_color();          // boot ncurses-color
    initcolors();           // initialize colors for color.h

    playlevel();

    endwin();               // close ncurses
    return 0;
}