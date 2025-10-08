#include "color.h"

// converts 3-bit color to ncurses constant
short decodecolor(int c) {
    switch(c) {
    case 0:
        return COLOR_BLACK; 
    case 1:
        return COLOR_BLUE; 
    case 2:
        return COLOR_GREEN; 
    case 3:
        return COLOR_CYAN; 
    case 4:
        return COLOR_RED; 
    case 5:
        return COLOR_MAGENTA; 
    case 6:
        return COLOR_YELLOW; 
    case 7:
        return COLOR_WHITE; 
    }
    return COLOR_WHITE;
}

// converts ncurses constant to 3-bit color
int encodecolor(short c) {
    switch(c) {
    case COLOR_BLACK: 
        return 0;
    case COLOR_BLUE: 
        return 1;
    case COLOR_GREEN: 
        return 2;
    case COLOR_CYAN: 
        return 3;
    case COLOR_RED: 
        return 4;
    case COLOR_MAGENTA: 
        return 5;
    case COLOR_YELLOW: 
        return 6;
    case COLOR_WHITE: 
        return 7;
    }
    return COLOR_WHITE;
}

// initializes every combo of 3-bit colors as colorpair
// the first four bits of the pair id are the background, the last four bits are the foreground
void initcolors() {
    for(int fg = 0; fg < 8; fg++)
        for(int bg = 0; bg < 8; bg++) {
            init_pair(bg*16+fg, decodecolor(fg), decodecolor(bg));
        }
}

// sets foreground (fg) and background (bg) with ncurses constants
void setcolor(WINDOW* win, short fg, short bg) {
    wattron(win, COLOR_PAIR(encodecolor(fg)+16*encodecolor(bg)));
}