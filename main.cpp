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
    for (int i = 0; i < 8; i++) {
        h.add(d.cards.back());
        d.cards.pop_back();
    }

    hand played;    
    while(true) {
        clear();
        h.print();
        played.print();
        refresh();
        switch (getchar()) {
            case 'q':
                return;
            case 'h':
                h.moveCursor(-1);
                break;
            case 'l':
                h.moveCursor(1);
                break;
            case ' ':
                h.selected[h.cursor] = !h.selected[h.cursor];
                break;
            case 'p':
                played = hand(h.popSelected());
                played.cursor = -1;
                for (int i = 0; i < played.selected.size(); i++) { //draw new cards
                    h.add(d.cards.back());
                    d.cards.pop_back();
                }
                break;
        }
    }
}

int main() {
    setlocale(LC_ALL, "");
    initscr();             // Start curses modeee
    start_color();
    initcolors();

    playlevel();
    refresh();             // Print it on to the real screen

    endwin();              // End curses mode
    return 0;
}