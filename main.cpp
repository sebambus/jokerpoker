#define _XOPEN_SOURCE_EXTENDED // needed to run unicode??
#include <ncursesw/ncurses.h>
#include <vector>
#include <locale>
#include "deck.h"
#include "card.h"
#include "color.h"
#include "hand.h"
#include "scorekeep.h"
#include "level.h"

void playlevel() {
    level l;
    l.setupLevel();

    while(true) {
        clear();
        l.printLevel();
        refresh();

        switch (getchar()) {
            case 'q': // quit
                return;
            case 'h': // left (vim)
                l.h.moveCursor(-1);
                break;
            case 'l': // right (vim)
                l.h.moveCursor(1);
                break;
            case ' ': // select
                l.h.selectCursor();
                break;
            case 'p': // play
                l.playHand();
                break;
            case 'd': // discard
                l.discardHand();
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