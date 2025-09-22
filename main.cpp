#include <ncurses.h>
#include "deck.h"
#include "card.h"

deck d;

void playlevel() {
    return;
}

int main() {
    initscr();             // Start curses modeee
    printw("Hello World !!!"); // Print Hello World
    refresh();             // Print it on to the real screen
    printw("Hello World !!!"); // Print Hello World
    refresh();             // Print it on to the real screen

    playlevel();

    endwin();              // End curses mode
    return 0;
}