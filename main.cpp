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
#include "shop.h"

int main() {
    setlocale(LC_ALL, "");  // switch to UTF-8??
    initscr();              // boot ncurses
    start_color();          // boot ncurses-color
    initcolors();           // initialize colors for color.h
    curs_set(0);            // hides cursor

    level l(new game(), 30);
    l.play();
    shop s;
    s.enter();

    endwin();               // close ncurses
    return 0;
}