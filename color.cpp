#include "color.h"

// initializes every combo of 3-bit colors as colorpair
// the first four bits of the pair id are the background, the last four bits are the foreground
void initcolors() {
    for(int fg = 0; fg < 8; fg++)
        for(int bg = 0; bg < 8; bg++) {
            init_pair(bg*16+fg, fg, bg);
        }
}

// sets foreground (fg) and background (bg) with ncurses constants
void setcolor(WINDOW* win, short fg, short bg) {
    wattron(win, COLOR_PAIR(fg+16*bg));
}

void unsetcolor(WINDOW* win, short fg, short bg){
    wattroff(win, COLOR_PAIR(fg+16*bg));
}