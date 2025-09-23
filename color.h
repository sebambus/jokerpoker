#pragma once
#include <ncurses.h>

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

void initcolors() {
    for(int fg = 0; fg < 8; fg++)
        for(int bg = 0; bg < 8; bg++) {
            init_pair(bg*16+fg, decodecolor(fg), decodecolor(bg));
        }
}

// sets color mode, first arg is text and second arg is background
void setcolor(short fg, short bg) {
    attron(COLOR_PAIR(encodecolor(fg)+16*encodecolor(bg)));
}