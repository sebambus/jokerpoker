#define _XOPEN_SOURCE_EXTENDED // needed to run unicode??
#include <ncursesw/ncurses.h>
#include <vector>
#include <locale>
#include <fstream>
#include "deck.h"
#include "card.h"
#include "color.h"
#include "hand.h"
#include "level.h"
#include "shop.h"
#include "game.h"
#include "debug.h"

int main() {
    clearDebug();

    setlocale(LC_ALL, "");  // switch to UTF-8??
    initscr();              // boot ncurses
    start_color();          // boot ncurses-color
    initcolors();           // initialize colors for color.h
    curs_set(0);            // hides cursor
    srand(time(NULL));

    game g;
    g.run();

    endwin();               // close ncurses
    return 0;
}