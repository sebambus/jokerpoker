#define _XOPEN_SOURCE_EXTENDED // needed to run unicode??
#include <ncursesw/ncurses.h>
#include <vector>
#include <locale>
#include "deck.h"
#include "card.h"
#include "color.h"
#include "hand.h"
#include "level.h"
#include "shop.h"
#include "game.h"

int main() {
    setlocale(LC_ALL, "");  // switch to UTF-8??
    initscr();              // boot ncurses
    start_color();          // boot ncurses-color
    initcolors();           // initialize colors for color.h
    curs_set(0);            // hides cursor
    srand(time(NULL));

    game g;

    while(g.ante <= 8) {
        shop s(&g);
        while(g.round <= 3) {
            if(g.round + g.ante > 2) {
                s.refresh();
                s.run();
            }
            level l(&g);
            g.round++;
        }
        g.ante++;
        g.round = 1;
    }

    endwin();               // close ncurses
    return 0;
}